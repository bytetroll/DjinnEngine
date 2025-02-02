//
// Created by Dev on 4/8/2018.
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
