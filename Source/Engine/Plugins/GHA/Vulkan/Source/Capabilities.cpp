#include <GHA/Vulkan/Capabilities.h>
#include <GHA/Format.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/Memory.h>

DJINN_NS3(GHA, Vulkan);

Capabilities::Capabilities(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ICapabilities(registry, outer, host) {

}

COM::Result Capabilities::Initialize(vk::PhysicalDevice &physicalDevice) {
    UInt32 diagnosticCounts[static_cast<int>(GHA::FormatCapability::eCount)];
    Core::Memory::Zero(diagnosticCounts, sizeof(UInt32) * static_cast<int>(GHA::FormatCapability::eCount));

    // Pool
    for (int i = 0; i < static_cast<int>(GHA::Format::eCount); i++) {
        vk::Format format = AsFormat(static_cast<GHA::Format>(i));

        // Get properties
        physicalDevice.getFormatProperties(format, &formatProperties[i]);

        // Diagnostic
        diagnosticCounts[0] += static_cast<bool>(formatProperties[i].optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);
        diagnosticCounts[1] += static_cast<bool>(formatProperties[i].bufferFeatures & vk::FormatFeatureFlagBits::eStorageTexelBuffer);
        diagnosticCounts[2] += static_cast<bool>(formatProperties[i].bufferFeatures & vk::FormatFeatureFlagBits::eVertexBuffer);
        diagnosticCounts[3] += static_cast<bool>(formatProperties[i].optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc);
        diagnosticCounts[4] += static_cast<bool>(formatProperties[i].optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitDst);
        diagnosticCounts[5] += static_cast<bool>(formatProperties[i].optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment);
    }

    // Diagnostic
    InfoLog(this).Write(
            "Pooled ", static_cast<int>(GHA::Format::eCount), " format properties with capabilities [",
            diagnosticCounts[0], " texture(s), ",
            diagnosticCounts[1], " texture buffer(s), ",
            diagnosticCounts[2], " vertex buffer(s), ",
            diagnosticCounts[3], " blitsrc(s), ",
            diagnosticCounts[4], " blitdst(s), ",
            diagnosticCounts[5], " render target(s)",
            "]");

    // OK
    return COM::kOK;
}


bool Capabilities::IsFormatSupported(GHA::Format format, const GHA::FormatCapabilitySet &capabilities) {
    bool ok = true;
    if (capabilities & GHA::FormatCapability::eTexture && !(formatProperties[static_cast<int>(format)].optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage)) ok = false;
    if (capabilities & GHA::FormatCapability::eTextureBuffer && !(formatProperties[static_cast<int>(format)].bufferFeatures & vk::FormatFeatureFlagBits::eStorageTexelBuffer)) ok = false;
    if (capabilities & GHA::FormatCapability::eVertexBuffer && !(formatProperties[static_cast<int>(format)].bufferFeatures & vk::FormatFeatureFlagBits::eVertexBuffer)) ok = false;
    if (capabilities & GHA::FormatCapability::eBlitSrc && !(formatProperties[static_cast<int>(format)].optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)) ok = false;
    if (capabilities & GHA::FormatCapability::eBlitDst && !(formatProperties[static_cast<int>(format)].optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitDst)) ok = false;
    if (capabilities & GHA::FormatCapability::eRenderTarget && !(formatProperties[static_cast<int>(format)].optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment)) ok = false;
    return ok;
}

USize Capabilities::GetFormatSize(GHA::Format format) {
    switch (format) {
        case GHA::Format::eUndefined:
            return 0;
        case GHA::Format::eA2R10G10B10Unorm:
            return 4;
        case GHA::Format::eR8G8B8A8Srgb:
            return 4;
        case GHA::Format::eR16G16B16A16F:
            return 8;
        case GHA::Format::eR16G16B16A16I:
            return 8;
        case GHA::Format::eR16G16B16A16SNorm:
            return 8;
        case GHA::Format::eR16G16B16A16U:
            return 8;
        case GHA::Format::eR16G16B16A16UNorm:
            return 8;
        case Format::eR32G32B32A32F:
            return 16;
        case Format::eR32G32B32A32I:
            return 16;
        case Format::eR32G32B32A32U:
            return 16;
        case Format::eR32G32F:
            return 8;
        case Format::eR32G32I:
            return 8;
        case Format::eR32G32U:
            return 8;
        case Format::eR8G8B8A8Snorm:
            return 4;
        case Format::eR8G8B8A8Unorm:
            return 4;
        case Format::eB8G8R8A8Unorm:
            return 4;
        case Format::eR32G32B32F:
            return 12;
        case Format::eR32G32B32I:
            return 12;
        case Format::eR32G32B32U:
            return 12;
        case Format::eR32F:
            return 4;
        case Format::eR32I:
            return 4;
        case Format::eR32U:
            return 4;
        case Format::eR8G8B8Snorm:
            return 3;
        case Format::eB8G8R8Snorm:
            return 3;
        case Format::eB8G8R8A8Snorm:
            return 4;
        case Format::eR8G8B8Unorm:
            return 3;
        case Format::eB8G8R8Unorm:
            return 3;
        case Format::eR16G16B16F:
            return 6;
        case Format::eR16G16B16U:
            return 6;
        case Format::eBC1:
            return 0;
        case Format::eBC2:
            return 0;
        case Format::eBC3:
            return 0;
        case Format::eBC4:
            return 0;
        case Format::eBC5:
            return 0;
        case Format::eBC6H:
            return 0;
        case Format::eBC7:
            return 0;
        case Format::eASTC:
            return 0;
        case Format::eR8Unorm:
            return 1;
        case Format::eD32F:
            return 4;
        case Format::eCount:
            break;
    }
    Always("Bad format value");
    return {};
}

bool Capabilities::SelectFormat(UInt32 count, GHA::Format *formats, const GHA::FormatCapabilitySet &capabilities, GHA::Format &out) {
    for (UInt32 i = 0; i < count; i++) {
        if (Capabilities::IsFormatSupported(formats[i], capabilities)) {
            out = formats[i];
            return true;
        }
    }
    return false;
}

COM::Result Capabilities::Released() {
    // OK
    return COM::kOK;
}
