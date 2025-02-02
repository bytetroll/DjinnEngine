#include <GHA/Vulkan/Adapter.h>
#include <GHA/Vulkan/Output.h>
#include <GHA/Vulkan/Capabilities.h>
#include <Core/Memory.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

Adapter::Adapter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAdapter(registry, outer, host) {

}

COM::Result Adapter::Initialize(vk::PhysicalDevice &physicalDevice) {
    this->physicalDevice = physicalDevice;

    // Get properties
    physicalDevice.getMemoryProperties(&memoryProperties);
    physicalDevice.getProperties(&properties);

    // Get queue family count
    UInt32 queueFamilyCount = 0;
    physicalDevice.getQueueFamilyProperties(&queueFamilyCount, nullptr);

    // Get queue families
    Core::Array<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
    physicalDevice.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.Ptr());

    // Create outputs
    for (UInt32 i = 0; i < static_cast<UInt32>(queueFamilies.Size()); i++) {
        Output *output;
        if (!registry->CreateClass(this, &output) || !output->Initialize(i, queueFamilies[i], physicalDevice)) {
            ErrorLog(this).Write("Failed to create vulkan display output");
            return COM::kError;
        }

        // ...
        outputs.Add(output);
    }

    // Create capabilities
    if (!registry->CreateClass(this, &capabilities) || !capabilities->Initialize(physicalDevice)) {
        ErrorLog(this).Write("Failed to create vulkan capabilities");
        return COM::kError;
    }

    // Create decoration
    decoration = Core::String(properties.deviceName);

    // Diagnostic
    InfoLog(this).Write("Initialized '", decoration, "'");

    // ...
    return COM::kOK;
}

Core::String Adapter::GetDecoration() {
    return decoration;
}

USize Adapter::GetMemory() {
    USize total = 0;
    for (UInt32 i = 0; i < memoryProperties.memoryHeapCount; i++) {
        total += memoryProperties.memoryHeaps[i].size;
    }
    return total;
}


COM::Result Adapter::GetOutputs(UInt32 &count, GHA::IOutput **adapters) {
    if (adapters) {
        Core::Memory::Copy(adapters, outputs.Ptr(), count * sizeof(GHA::IOutput *));
    } else {
        count = static_cast<UInt32>(outputs.Size());
    }
    return COM::kOK;
}

GHA::ICapabilities *Adapter::GetCapabilities() {
    return capabilities;
}

COM::Result Adapter::Released() {
    // OK
    return COM::kOK;
}
