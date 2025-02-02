#include <GHA/Vulkan/Shader.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>

DJINN_NS3(GHA, Vulkan);

Shader::Shader(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IShader(registry, outer, host) {

}


COM::Result Shader::Initialize(const GHA::ShaderDesc& desc, CompiledShaderBlob* blob, vk::Device device) {
    this->desc = desc;
    this->blob = blob;
    this->device = device;

    // Attempt to create shader module
    vk::ShaderModuleCreateInfo info;
    info.codeSize = blob->blob.ByteSize();
    info.pCode = blob->blob.Ptr();
    if (vk::CheckedResult result = device.createShaderModule(&info, nullptr, &module)) {
        ErrorLog(this).Write("Failed to create vulkan shader: ", vk::to_string(result).c_str());
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

const ShaderDesc& Shader::GetDesc() {
    return desc;
}

ICompiledShaderBlob *Shader::GetBlob() {
    return blob;
}

COM::Result Shader::Released() {
    device.destroyShaderModule(module, nullptr);

    // OK
    return COM::kOK;
}
