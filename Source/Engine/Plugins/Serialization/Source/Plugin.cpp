//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/16/2017.
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
