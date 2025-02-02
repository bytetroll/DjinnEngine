//
// Created by Dev on 4/5/2018.
//

#include <Data/MaterialCompiler.h>
#include <Host/IRegistry.h>
#include <Data/BaseMaterialAsset.h>
#include <Data/MaterialCompilerPool.h>
#include <Data/IShaderCompiler.h>
#include <Base/DefaultLogs.h>
#include <NyLang/Core/Host/Registry.h>
#include <Data/NyShader/MaterialOperations.h>
#include <Async/Group.h>
#include <Async/Async.h>
#include <Data/IMaterialLayout.h>
#include <Data/IMaterialType.h>
#include <Core/StringStream.h>
#include <Core/MemoryStream.h>

DJINN_NS2(Data);

MaterialCompiler::MaterialCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialCompiler(registry, outer, host) {

}

COM::Result MaterialCompiler::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&shaderCompiler)) {
        ErrorLog(this).Write("Expected shader compiler to be installed");
        return COM::kError;
    }

    if (!registry->CreateClass(this, &defaultPool) || !defaultPool->Initialize()) {
        ErrorLog(this).Write("Failed to create material shader compiler pool");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result MaterialCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = static_cast<MaterialConfiguration *>(_config);

    // Must have a layout or type
    if (layouts.Size() == 0 || types.Size() == 0) {
        ErrorLog(this).Write("Failed to compile material, must have at least one layout and at least one type registered");
        return COM::kError;
    }

    // ...
    Platform::SpinMutex mutex;

    // Create asset
    BaseMaterialAsset *asset;
    if (!this->registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    // Read stream as text
    Core::String source = Core::StringStream(stream).ReadUntil(0);

    // Async result
    COM::Result result = COM::kOK;

    // Schedule all layouts
    Async::Group group;
    Async::Foreach(group, layouts.Size(), [&](USize i) {
        auto layout = layouts[i];

        // Schedule all types
        Async::Foreach(group, types.Size(), [&, layout](USize j) {
            auto type = types[j];

            // Pool registry
            NyLang::Registry *registry;
            if (!defaultPool->Pool(&registry)) {
                Platform::LockGuard guard(mutex);
                result = COM::kError;
                return;
            }

            // Append kernel
            Core::String kernelSource = source + "\n__include(\"" + type->GetKernelPath() + "\");\n";

            // Memory stream
            Core::MemoryStream kernelStream(kernelSource.Ptr());

            // Configure inbuilts
            auto inbuilts = registry->GetClass<NyShader::MaterialInbuilts>();
            inbuilts->layoutPath = layout->GetPath();
            inbuilts->typePath = type->GetHeaderPath();

            // Base configuration
            ShaderConfiguration shaderConfiguration;

            // Compile shader
            ShaderAsset *shaderAsset;
            if (!shaderCompiler->Compile(registry, &kernelStream, &shaderConfiguration, reinterpret_cast<IAsset **>(&shaderAsset))) {
                Platform::LockGuard guard(mutex);
                result = COM::kError;
                return;
            }

            {
                MaterialQuery query;
                query.type = type->GetClassID();
                query.layout = layout->GetClassID();

                Platform::LockGuard guard(mutex);
                asset->shaders[query.Hash()] = shaderAsset;
            }

            // Done
            defaultPool->Push(registry);
        }, Async::kResource);
    }, Async::kResource);

    // Wait
    group.Wait();

    // Good?
    if (!result) {
        return result;
    }

    // ...
    *out = asset;
    return COM::kOK;
}

COM::Result MaterialCompiler::Register(IMaterialLayout *layout) {
    layouts.Add(layout);
    return COM::kOK;
}

COM::Result MaterialCompiler::Register(IMaterialType *type) {
    types.Add(type);
    return COM::kOK;
}
