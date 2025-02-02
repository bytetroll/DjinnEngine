//
// Created by Dev on 4/8/2018.
//

#include <Graphics/ScreenOutput.h>
#include <Graphics/IGraphicsSystem.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <GHA/IDevice.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Graphics/PassBuilder.h>
#include <Graphics/ID.h>

DJINN_NS2(Graphics);

ScreenOutput::ScreenOutput(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IOutput(registry, outer, host) {

}

COM::Result ScreenOutput::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("Expected graphics system to be installed");
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result ScreenOutput::Configure(PassBuilder builder) {
    texture = builder.AddTexture(
            GHA::TextureDesc()
                    .Size(system->GetSwapchain()->GetTexture()->GetDesc().size)
                    .Type(GHA::TextureType::e2D)
                    .Format(system->GetSwapchain()->GetTexture()->GetDesc().format)
                    .Usage(GHA::TextureUsage::eColor)
                    .Flags(GHA::TextureFlag::eClearable)
    );
    return COM::kOK;
}

Texture ScreenOutput::GetTexture(PassBuilder builder) {
    return texture;
}

GHA::Size ScreenOutput::GetSize() {
    return system->GetSwapchain()->GetTexture()->GetDesc().size;
}
