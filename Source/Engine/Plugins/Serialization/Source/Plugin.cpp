//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Serialization/ObjectSerializer.h>
#include <Serialization/SerializerRegistry.h>
#include <Serialization/SerializerHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Serialization);

DJINN_C_EXPORT_PLUGINS_SERIALIZATION COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        SerializerRegistry* serializerRegistry;
        if (!registry->CreateClass(plugin, &serializerRegistry) || !registry->GetPipeline()->AddInterface(serializerRegistry)) {
            ErrorLog(registry).Write("Failed to install serialization registry");
            return COM::kError;
        }

        SerializerHost* serializerHost;
        if (!registry->CreateClass(plugin, &serializerHost) || !serializerHost->Initialize() || !registry->GetPipeline()->AddInterface(serializerHost)) {
            ErrorLog(registry).Write("Failed to install serialization worldHost");
            return COM::kError;
        }

        ObjectSerializer* objectSerializer;
        if (!registry->CreateClass(plugin, &objectSerializer) || !objectSerializer->Initialize() || !serializerRegistry->AddSerializer(objectSerializer)) {
            ErrorLog(registry).Write("Failed to install object serializer");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_SERIALIZATION COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    Check(registry->GetPipeline()->RemoveInterface(ISerializerRegistry::kIID));
    Check(registry->GetPipeline()->RemoveInterface(ISerializerHost::kIID));
    return COM::kOK;
}
