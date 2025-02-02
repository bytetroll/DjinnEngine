//
// Created by Dev on 2/24/2018.
//

#include <Graphics/SceneProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>
#include <Graphics/IGraphicsSystem.h>

DJINN_NS2(Graphics);

SceneProcess::SceneProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneProcess(registry, outer, host) {

}

COM::Result SceneProcess::Initialize() {
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

COM::Result SceneProcess::Build(ScenePacket *packet, Graphics::PassBuilder builder, double delta) {
    this->packet = packet;

    // ...
    return pipeline->Build(builder, delta);
}


IPipeline *SceneProcess::GetPipeline() {
    return pipeline;
}

ScenePacket *SceneProcess::GetPacket() {
    return packet;
}

