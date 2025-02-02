//
// Created by Dev on 2/24/2018.
//

#include <Graphics/WorldProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>

DJINN_NS2(Graphics);

WorldProcess::WorldProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldProcess(registry, outer, host) {

}

COM::Result WorldProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize world process, could not create pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result WorldProcess::Build(WorldPacket* packet, Graphics::PassBuilder builder, double delta) {
    this->packet = packet;
    return pipeline->Build(builder, delta);
}

IPipeline *WorldProcess::GetPipeline() {
    return pipeline;
}

WorldPacket *WorldProcess::GetPacket() {
    return packet;
}
