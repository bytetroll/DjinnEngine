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
