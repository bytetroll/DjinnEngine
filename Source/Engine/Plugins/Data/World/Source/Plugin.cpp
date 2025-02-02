//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Serialization/ISerializerRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/BaseWorldAsset.h>
#include <Data/BaseWorldSerializer.h>
#include <Data/WorldConfig.h>
#include <Data/WorldConfigSerializer.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_WORLD COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        Serialization::ISerializerRegistry* serializerRegistry;
        if (!registry->GetPipeline()->GetInterface(&serializerRegistry)) {
            ErrorLog(registry).Write("Failed to install world data plugin, expected serializer registry to be installed");
            return COM::kError;
        }

        BaseWorldSerializer *baseWorldSerializer;
        if (!registry->CreateClass(plugin, &baseWorldSerializer) || !serializerRegistry->AddSerializer(BaseWorldAsset::kCID, baseWorldSerializer)) {
            ErrorLog(registry).Write("Failed to install base world serializer");
            return COM::kError;
        }

        WorldConfigSerializer *worldConfigSerializer;
        if (!registry->CreateClass(plugin, &worldConfigSerializer) || !serializerRegistry->AddSerializer(WorldConfig::kCID, worldConfigSerializer)) {
            ErrorLog(registry).Write("Failed to install world config serializer");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_WORLD COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
