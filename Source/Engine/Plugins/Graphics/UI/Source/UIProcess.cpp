//
// Created by Dev on 2/24/2018.
//

#include <Graphics/UIProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>
#include <Graphics/IGraphicsSystem.h>

DJINN_NS2(Graphics);

UIProcess::UIProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIProcess(registry, outer, host) {

}

COM::Result UIProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize ui process, could not create pipeline");
        return COM::kError;
    }

    // Get system
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("UIDrawPass expected an installed graphics system");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result UIProcess::Build(UIPacket *packet, const Texture& depthTexture, Graphics::PassBuilder builder, double delta) {
    this->packet = packet;
    this->depthTexture = depthTexture;

    // ...
    return pipeline->Build(builder, delta);
}


IPipeline *UIProcess::GetPipeline() {
    return pipeline;
}

UIPacket *UIProcess::GetPacket() {
    return packet;
}

Texture UIProcess::GetDepthTexture() {
    return depthTexture;
}
