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
 on 3/6/2018.
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
