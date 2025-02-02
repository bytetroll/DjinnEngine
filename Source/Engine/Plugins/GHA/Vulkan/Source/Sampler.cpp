#include <GHA/Vulkan/Sampler.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS3(GHA, Vulkan);

Sampler::Sampler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ISampler(registry, outer, host) {

}

COM::Result Sampler::Initialize(const GHA::SamplerDesc &desc, vk::Device device) {
    this->desc = desc;
    this->device = device;

    // Attempt to create sampler
    vk::SamplerCreateInfo info;
    info.minFilter = AsFilter(desc.minFilter);
    info.magFilter = AsFilter(desc.magFilter);
    info.mipmapMode = AsMipMapMode(desc.mipMapMode);
    info.addressModeU = AsAddressMode(desc.addressU);
    info.addressModeV = AsAddressMode(desc.addressV);
    info.addressModeW = AsAddressMode(desc.addressW);
    info.mipLodBias = desc.biasLOD;
    info.anisotropyEnable = static_cast<vk::Bool32>(desc.anisotropy);
    info.maxAnisotropy = desc.maxAnisotropy;
    info.compareEnable = static_cast<vk::Bool32>(desc.compare);
    info.compareOp = AsCompareOp(desc.compareOp);
    info.minLod = desc.minLOD;
    info.maxLod = desc.maxLOD;
    if (vk::CheckedResult result = device.createSampler(&info, nullptr, &sampler)) {
        ErrorLog(this).Write("Failed to create vulkan sampler: ", vk::to_string(result).c_str());
        return COM::kOK;
    }

    // Prepare descriptor
    descriptor.imageView = nullptr;
    descriptor.sampler = sampler;

    // OK
    return COM::kOK;
}

const GHA::SamplerDesc &Sampler::GetDesc() {
    return desc;
}

COM::Result Sampler::Released() {
    device.destroySampler(sampler, nullptr);

    // OK
    return COM::kOK;
}