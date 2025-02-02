//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <GHA/Vulkan/ShaderNILCompiler.h>
#include <Data/IShaderCompiler.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <Serialization/ISerializerRegistry.h>
#include <GHA/Vulkan/CompiledShaderBlobSerializer.h>

DJINN_NS3(GHA, Vulkan);

DJINN_C_EXPORT_PLUGINS_GHA_VULKANSHADER COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        Data::IShaderCompiler* shaderCompiler;
        if (!registry->GetPipeline()->GetInterface(&shaderCompiler)) {
            ErrorLog(registry).Write("Failed to install plugin, expected shader compiler to be installed");
            return COM::kError;
        }

        Vulkan::ShaderNILCompiler *shaderNILCompiler;
        if (!registry->CreateClass(plugin, &shaderNILCompiler) || !shaderCompiler->AddNILCompiler(shaderNILCompiler)) {
            ErrorLog(registry).Write("Failed to install shader compiler");
            return COM::kError;
        }

        Serialization::ISerializerRegistry* serializerRegistry;
        if (!registry->GetPipeline()->GetInterface(&serializerRegistry)) {
            ErrorLog(registry).Write("Failed to install plugin, expected serializer registry to be installed");
            return COM::kError;
        }

        CompiledShaderBlobSerializer *serializer;
        if (!registry->CreateClass(plugin, &serializer) || !serializerRegistry->AddSerializer(CompiledShaderBlob::kCID, serializer)) {
            ErrorLog(registry).Write("Failed to install compiled shader blob serializer");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GHA_VULKANSHADER COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
