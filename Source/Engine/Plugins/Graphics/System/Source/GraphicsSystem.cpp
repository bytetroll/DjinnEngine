//
// Created by Dev on 1/10/2018.
//

#include <Graphics/GraphicsSystem.h>
#include <GHA/IConfiguration.h>
#include <GHA/IInstance.h>
#include <GHA/IDevice.h>
#include <GHA/ISwapchain.h>
#include <GHA/IScheduler.h>
#include <GHA/IInterpreter.h>
#include <Base/DefaultLogs.h>
#include <Platform/Window.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/Graph.h>

DJINN_NS2(Graphics);

GraphicsSystem::GraphicsSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IGraphicsSystem(registry, outer, host) {

}

COM::Result GraphicsSystem::Initialize() {
    // Create graph
    if (!registry->CreateClass(this, &graph) || !graph->Initialize()) {
        ErrorLog(this).Write("Failed to create graph");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");
    return COM::kOK;
}

COM::Result GraphicsSystem::Configure(GHA::IConfiguration *configuration, Platform::Window *window) {
    this->configuration = configuration;
    this->window = window;

    // Create instance
    if (!registry->CreateClass(configuration->GetInstanceCID(), this, &instance) || !instance->Initialize(
            GHA::InstanceDesc()
            //.Flags(GHA::InstanceFlag::eDebug)
    )) {
        ErrorLog(this).Write("Configuration failed, could not create gha instance");
        return COM::kError;
    }

    // Get adapters
    UInt32 adapterCount;
    if (!instance->GetAdapters(adapterCount, nullptr)) {
        ErrorLog(this).Write("Configuration failed, could not get adapters");
        return kNoAdapters;
    }

    // ...
    if (adapterCount == 0) {
        ErrorLog(this).Write("Configuration failed, no suitable adapters");
        return kNoAdapters;
    }

    // Get adapters
    Core::Array<GHA::IAdapter *> adapters(adapterCount);
    if (!instance->GetAdapters(adapterCount, adapters.Ptr())) {
        ErrorLog(this).Write("Configuration failed, could not get adapters");
        return kNoAdapters;
    }

    // Create device
    if (!instance->CreateDevice(
            this,
            adapters[0],
            GHA::DeviceDesc()
            //.Flags(GHA::DeviceFlag::eDebug)
            ,
            &device)) {
        ErrorLog(this).Write("Configuration failed, failed to initialize device");
        return COM::kError;
    }

    // Window size
    auto size = window->GetSize();

    // Create swapchain
    if (!device->CreateSwapchain(
            this,
            GHA::SwapchainDesc()
                    .WindowHandle(window->GetHandle())
                    .Dimensions(GHA::Size(static_cast<UInt32>(size.x), static_cast<UInt32>(size.y)))
                    /*.SwapMode(GHA::SwapMode::eFIFO)*/,
            &swapchain
    )) {
        ErrorLog(this).Write("Configuration failed, failed to initialize device");
        return COM::kError;
    }

    // Get supported schedulers
    UInt32 schedulerCount;
    if (!configuration->GetSupportedSchedulerCIDs(schedulerCount, nullptr)) {
        ErrorLog(this).Write("Configuration failed, could not get supported scheduler class ids");
        return COM::kError;
    }

    // Must support one
    if (schedulerCount == 0) {
        ErrorLog(this).Write("Configuration failed, provided gha configuration reports no supported schedulers");
        return COM::kError;
    }

    // Get supported schedulers
    Core::Array<COM::ClassID> schedulerIDs(schedulerCount);
    if (!configuration->GetSupportedSchedulerCIDs(schedulerCount, schedulerIDs.Ptr())) {
        ErrorLog(this).Write("Configuration failed, could not get supported scheduler class ids");
        return COM::kError;
    }

    // Try all
    for (USize i = 0; i < schedulerIDs.Size() && !scheduler; i++) {
        if (!registry->HasClass(schedulerIDs[i])) {
            continue;
        }

        // Get supported interpreters
        UInt32 interpreterCount;
        if (!configuration->GetSupportedInterpreterCIDs(schedulerIDs[i], interpreterCount, nullptr) || interpreterCount == 0) {
            continue;
        }

        // Get supported schedulers
        Core::Array<COM::ClassID> interpreterIDs(interpreterCount);
        if (!configuration->GetSupportedInterpreterCIDs(schedulerIDs[i], interpreterCount, interpreterIDs.Ptr())) {
            continue;
        }

        // Attempt to create
        for (auto cid : interpreterIDs) {
            if (registry->HasClass(cid)) {
                // Create classes
                if (!registry->CreateClass(schedulerIDs[i], this, &scheduler) ||
                    !registry->CreateClass(cid, this, &interpreter)) {
                    ErrorLog(this).Write("Configuration failed, failed to create scheduler or interpreter from configuration");
                    return COM::kError;
                }

                // Initialize
                if (!scheduler->Initialize(device, swapchain) || !interpreter->Initialize(instance, adapters[0], device, swapchain)) {
                    ErrorLog(this).Write("Configuration failed, failed to initialize scheduler or interpreter");
                    return COM::kError;
                }

                // ...
                break;
            }
        }
    }

    // ...
    if (!scheduler || !interpreter) {
        ErrorLog(this).Write("Configuration failed, invalid configuration, no valid scheduler interpreter combination");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Configured to configuration ", configuration->GetClassName());

    // OK
    return COM::kOK;
}

COM::Result GraphicsSystem::Run(Game::StepMode mode, double delta) {
    // Compile graph
    if (!graph->Compile(scheduler, delta)) {
        ErrorLog(this).Write("Failed to compile graphics graph");
        return COM::kError;
    }

    // Compile scheduler
    if (!scheduler->Compile()) {
        ErrorLog(this).Write("Failed to compile graphics scheduler");
        return COM::kError;
    }

    // Compile interpreter
    if (!interpreter->Compile(scheduler)) {
        ErrorLog(this).Write("Failed to compile graphics interpreter");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

Game::StepModeSet GraphicsSystem::GetStepMode() {
    return Game::StepMode::eAsync;
}

GHA::IInstance *GraphicsSystem::GetInstance() {
    return instance;
}

GHA::IDevice *GraphicsSystem::GetDevice() {
    return device;
}

GHA::ISwapchain *GraphicsSystem::GetSwapchain() {
    return swapchain;
}

GHA::IConfiguration *GraphicsSystem::GetConfiguration() {
    return configuration;
}

GHA::IScheduler *GraphicsSystem::GetScheduler() {
    return scheduler;
}

GHA::IInterpreter *GraphicsSystem::GetInterpreter() {
    return interpreter;
}

IGraph *GraphicsSystem::GetGraph() {
    return graph;
}

COM::Result GraphicsSystem::Configure(Base::PipelineBuilder& builder) {
    return COM::kOK;
}
