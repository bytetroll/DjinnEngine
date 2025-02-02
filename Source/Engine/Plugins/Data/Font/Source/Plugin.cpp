//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Data/BaseFontAsset.h>
#include <Data/BaseFontSerializer.h>
#include <Serialization/ISerializerRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/FontCompiler.h>
#include <Data/IAssetCompilerRegistry.h>
#include <Data/FontFileCompiler.h>
#include <Data/IAssetHost.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_FONT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
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

        BaseFontSerializer *baseFontSerializer;
        if (!registry->CreateClass(plugin, &baseFontSerializer) || !serializerRegistry->AddSerializer(BaseFontAsset::kCID, baseFontSerializer)) {
            ErrorLog(registry).Write("Failed to install base font serializer");
            return COM::kError;
        }

        FontCompiler *fontCompiler;
        if (!registry->CreateClass(plugin, &fontCompiler) || !fontCompiler->Initialize() || !compilerRegistry->AddCompiler(FontConfiguration::kCID, fontCompiler)) {
            ErrorLog(registry).Write("Failed to install font compiler");
            return COM::kError;
        }

        FontFileCompiler *fontAssetCompiler;
        if (!registry->CreateClass(plugin, &fontAssetCompiler) || !fontAssetCompiler->Initialize(host)) {
            ErrorLog(registry).Write("Failed to install font file asset compiler");
            return COM::kError;
        }

        // Install by extensions
        host->Register(".otf", fontAssetCompiler);
        host->Register(".ttf", fontAssetCompiler);
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_FONT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
