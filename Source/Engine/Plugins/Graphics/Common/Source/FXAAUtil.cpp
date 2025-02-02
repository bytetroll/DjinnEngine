//
// Created by Dev on 3/6/2018.
//

#include <Graphics/FXAAUtil.h>
#include <Data/ShaderConfiguration.h>
#include <Data/ShaderAsset.h>
#include <Math/Vector2.h>
#include <Math/Vector4.h>
#include <Data/IAssetHost.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Graphics);

FXAAUtil::FXAAUtil(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result FXAAUtil::Initialize() {
    // Get asset worldHost
    Data::IAssetHost* host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("FXAAUtil expected an asset worldHost to be installed");
        return COM::kError;
    }

    // Compile shader
    shaderAsset = host->CompileAssetAsync<Data::ShaderAsset>("Runtime/Engine/Shaders/FXAAUtil.dsf"_path);

    // OK
    return COM::kOK;
}


COM::Result FXAAUtil::Build(PassBuilder builder, const Texture &source, const Texture &dest) {
    // Prepare data
    struct Data {
        Math::Vector4 screenSize;
    } data = {
            {static_cast<float>(dest->size.width), static_cast<float>(dest->size.height), 0, 0}
    };

    // Bind.
    builder.BindShader(shaderAsset);
    builder.BindPacked("Data.data"_id, &data);
    builder.BindTexture("Resources.source"_id, source, GHA::TextureViews::none, GHA::Samplers::noMip);
    builder.BindRenderTarget(0, dest, GHA::TextureViews::none, GHA::BlendTargets::noBlend);
    builder.SetViewport({dest->size, 0, 1});
    builder.SetScissor(dest->size);
    builder.SetRasterizerState(GHA::Rasterizers::noCull);

    // Draw
    builder.Draw(3);

    // OK
    return COM::kOK;
}
