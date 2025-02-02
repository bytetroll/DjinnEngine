//
// Created by Dev on 2/24/2018.
//

#include <Graphics/CameraProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>

DJINN_NS2(Graphics);

CameraProcess::CameraProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ICameraProcess(registry, outer, host) {

}

COM::Result CameraProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize camera process, could not create pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result CameraProcess::Build(CameraPacket *packet, Graphics::PassBuilder builder, double delta) {
    this->packet = packet;
    return pipeline->Build(builder, delta);
}

IPipeline *CameraProcess::GetPipeline() {
    return pipeline;
}

CameraPacket *CameraProcess::GetPacket() {
    return packet;
}
