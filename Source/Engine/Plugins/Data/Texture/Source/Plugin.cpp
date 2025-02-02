//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Data/BaseTextureAsset.h>
#include <Data/BaseTextureSerializer.h>
#include <Serialization/ISerializerRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/TextureCompiler.h>
#include <Data/IAssetCompilerRegistry.h>
#include <Data/TextureFileCompiler.h>
#include <Data/IAssetHost.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_TEXTURE COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        Serialization::ISerializerRegistry* serializerRegistry;
        if (!registry->GetPipeline()->GetInterface(&serializerRegistry)) {
            ErrorLog(registry).Write("Failed to install plugin, expected serializer registry to be installed");
            return COM::kError;
        }

        Data::IAssetCompilerRegistry* compilerRegistry;
        if (!registry->GetPipeline()->GetInterface(&compilerRegistry)) {
            ErrorLog(registry).Write("Failed to install plugin, expected asset compiler registry to be installed");
            return COM::kError;
        }

        Data::IAssetHost* host;
        if (!registry->GetPipeline()->GetInterface(&host)) {
            ErrorLog(registry).Write("Failed to install plugin, expected asset worldHost to be installed");
            return COM::kError;
        }

        BaseTextureSerializer *baseTextureSerializer;
        if (!registry->CreateClass(plugin, &baseTextureSerializer) || !serializerRegistry->AddSerializer(BaseTextureAsset::kCID, baseTextureSerializer)) {
            ErrorLog(registry).Write("Failed to install base Texture serializer");
            return COM::kError;
        }

        TextureCompiler *textureCompiler;
        if (!registry->CreateClass(plugin, &textureCompiler) || !textureCompiler->Initialize() || !compilerRegistry->AddCompiler(TextureConfiguration::kCID, textureCompiler)) {
            ErrorLog(registry).Write("Failed to install texture compiler");
            return COM::kError;
        }

        TextureFileCompiler *textureAssetCompiler;
        if (!registry->CreateClass(plugin, &textureAssetCompiler) || !textureAssetCompiler->Initialize(host)) {
            ErrorLog(registry).Write("Failed to install font file asset compiler");
            return COM::kError;
        }

        // Install by extensions
        host->Register(".png", textureAssetCompiler);
        host->Register(".jpg", textureAssetCompiler);
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_TEXTURE COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
