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
#include <Data/BaseMaterialAsset.h>
#include <Data/BaseMaterialSerializer.h>
#include <Serialization/ISerializerRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/MaterialCompiler.h>
#include <Data/IAssetCompilerRegistry.h>
#include <Data/MaterialFileCompiler.h>
#include <Data/IAssetHost.h>
#include <Data/MaterialCompilerPool.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_MATERIAL COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
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

        BaseMaterialSerializer *shaderSerializer;
        if (!registry->CreateClass(plugin, &shaderSerializer) || !serializerRegistry->AddSerializer(BaseMaterialAsset::kCID, shaderSerializer)) {
            ErrorLog(registry).Write("Failed to install base material serializer");
            return COM::kError;
        }

        MaterialCompiler *shaderCompiler;
        if (!registry->CreateClass(plugin, &shaderCompiler) ||
            !shaderCompiler->Initialize() ||
            !compilerRegistry->AddCompiler(MaterialConfiguration::kCID, shaderCompiler) ||
            !registry->GetPipeline()->AddInterface(shaderCompiler)) {
            ErrorLog(registry).Write("Failed to install material compiler");
            return COM::kError;
        }

        MaterialFileCompiler *materialAssetCompiler;
        if (!registry->CreateClass(plugin, &materialAssetCompiler) || !materialAssetCompiler->Initialize(host)) {
            ErrorLog(registry).Write("Failed to install material file asset compiler");
            return COM::kError;
        }

        // Install by extensions
        host->Register(".dmf", materialAssetCompiler);
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_MATERIAL COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
