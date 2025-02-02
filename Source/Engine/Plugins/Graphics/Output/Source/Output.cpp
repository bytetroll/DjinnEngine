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
 on 4/8/2018.
//

#include <Graphics/Output.h>
#include <Graphics/IGraphicsSystem.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <GHA/IDevice.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Graphics/PassBuilder.h>
#include <Graphics/ID.h>

DJINN_NS2(Graphics);

Output::Output(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IOutput(registry, outer, host) {

}

COM::Result Output::Initialize(const GHA::Size &size) {
    IGraphicsSystem *system;
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("Expected graphics system to be installed");
        return COM::kError;
    }

    // Attempt to create texture
    if (!system->GetDevice()->CreateTexture(
            this,
            GHA::TextureDesc()
                    .Size(size)
                    .Type(GHA::TextureType::e2D)
                    .Format(system->GetSwapchain()->GetTexture()->GetDesc().format)
                    .Usage(GHA::TextureUsage::eColor)
                    .Flags(GHA::TextureFlag::eClearable),
            &texture
    )) {
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result Output::Configure(PassBuilder builder) {
    return COM::kOK;
}

Texture Output::GetTexture(PassBuilder builder) {
    return builder.AddTexture(texture);
}

GHA::Size Output::GetSize() {
    return texture->GetDesc().size;
}
