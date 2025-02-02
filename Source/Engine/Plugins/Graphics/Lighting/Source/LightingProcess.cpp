//
// Created by Dev on 2/24/2018.
//

#include <Graphics/LightingProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>

DJINN_NS2(Graphics);

LightingProcess::LightingProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILightingProcess(registry, outer, host) {

}

COM::Result LightingProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize lighting process, could not create pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result LightingProcess::Build(CameraPacket* packet, const LightingResources& resources, Graphics::PassBuilder builder, double delta) {
    this->packet = packet;
    this->resources = resources;
    return pipeline->Build(builder, delta);
}

IPipeline *LightingProcess::GetPipeline() {
    return pipeline;
}

CameraPacket *LightingProcess::GetPacket() {
    return packet;
}

const LightingResources &LightingProcess::GetResources() {
    return resources;
}
