//
// Created by Dev on 12/21/2017.
//

#include <Host/IRegistry.h>
#include <GHA/Vulkan/Configuration.h>
#include <GHA/Vulkan/Instance.h>
#include <GHA/Vulkan/Interpreter.h>
#include <GHA/PipelineScheduler/Scheduler.h>

DJINN_NS3(GHA, Vulkan);

Configuration::Configuration(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IConfiguration(registry, outer, host) {

}

Core::String Configuration::GetDisplayName() {
    return "Vulkan";
}

COM::ClassID Configuration::GetInstanceCID() {
    return Instance::kCID;
}

COM::Result Configuration::GetSupportedSchedulerCIDs(UInt32 &count, COM::ClassID *out) {
    if (out) {
        *out = PipelineScheduler::Scheduler::kCID;
    } else {
        count = 1;
    }
    return COM::kOK;
}

COM::Result Configuration::GetSupportedInterpreterCIDs(const COM::ClassID &scheduler, UInt32 &count, COM::ClassID *out) {
    if (out) {
        if (count == 1) {
            *out = Interpreter::kCID;
        }
    } else {
        count = 1U * (scheduler == PipelineScheduler::Scheduler::kCID);
    }
    return COM::kOK;
}

COM::Result Configuration::Released() {
    // OK
    return COM::kOK;
}

