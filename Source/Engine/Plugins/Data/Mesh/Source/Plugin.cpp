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
#include <Data/BaseMeshAsset.h>
#include <Data/BaseMeshSerializer.h>
#include <Serialization/ISerializerRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/SkeletalMeshAsset.h>
#include <Data/SkeletalMeshSerializer.h>
#include <Data/MeshCompiler.h>
#include <Data/MeshCollectionSerializer.h>
#include <Data/MeshCollectionAsset.h>
#include <Data/IAssetCompilerRegistry.h>
#include <Data/MeshFileCompiler.h>
#include <Data/IAssetHost.h>
#include <Data/BaseMaterialLayout.h>
#include <Data/IMaterialCompiler.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_MESH COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration.
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

       Data::IMaterialCompiler* materialCompiler;
        if (!registry->GetPipeline()->GetInterface(&materialCompiler)) {
            ErrorLog(registry).Write("Failed to install plugin, expected material compiler to be installed");
            return COM::kError;
        }

        Data::IAssetHost* host;
        if (!registry->GetPipeline()->GetInterface(&host)) {
            ErrorLog(registry).Write("Failed to install plugin, expected asset worldHost to be installed");
            return COM::kError;
        }

        BaseMeshSerializer *baseMeshSerializer;
        if (!registry->CreateClass(plugin, &baseMeshSerializer) || !serializerRegistry->AddSerializer(BaseMeshAsset::kCID, baseMeshSerializer)) {
            ErrorLog(registry).Write("Failed to install base mesh serializer");
            return COM::kError;
        }

        SkeletalMeshSerializer *skeletalMeshSerializer;
        if (!registry->CreateClass(plugin, &skeletalMeshSerializer) || !serializerRegistry->AddSerializer(SkeletalMeshAsset::kCID, skeletalMeshSerializer)) {
            ErrorLog(registry).Write("Failed to install skeletal mesh serializer");
            return COM::kError;
        }

        MeshCollectionSerializer *meshCollectionSerializer;
        if (!registry->CreateClass(plugin, &meshCollectionSerializer) || !serializerRegistry->AddSerializer(MeshCollectionAsset::kCID, meshCollectionSerializer)) {
            ErrorLog(registry).Write("Failed to install mesh collection serializer");
            return COM::kError;
        }

        MeshCompiler *meshCompiler;
        if (!registry->CreateClass(plugin, &meshCompiler) || !meshCompiler->Initialize() || !compilerRegistry->AddCompiler(MeshConfiguration::kCID, meshCompiler)) {
            ErrorLog(registry).Write("Failed to install mesh compiler");
            return COM::kError;
        }

        MeshFileCompiler *meshAssetCompiler;
        if (!registry->CreateClass(plugin, &meshAssetCompiler) || !meshAssetCompiler->Initialize(host)) {
            ErrorLog(registry).Write("Failed to install font file asset compiler");
            return COM::kError;
        }

        // Install by extensions
        host->Register(".obj", meshAssetCompiler);

        BaseMaterialLayout* baseLayout;
        if (!registry->CreateClass(plugin, &baseLayout) || !materialCompiler->Register(baseLayout)) {
            ErrorLog(registry).Write("Failed to install base material layout");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_MESH COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
