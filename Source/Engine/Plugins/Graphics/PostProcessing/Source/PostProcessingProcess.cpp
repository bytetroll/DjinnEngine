//
// Created by Dev on 2/24/2018.
//

#include <Graphics/PostProcessingProcess.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/Pipeline.h>

DJINN_NS2(Graphics);

PostProcessingProcess::PostProcessingProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPostProcessingProcess(registry, outer, host) {

}

COM::Result PostProcessingProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize post processing process, could not create pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result PostProcessingProcess::Build(CameraPacket* packet, const PostProcessingResources& resources, Graphics::PassBuilder builder, double delta) {
    this->resources = resources;
    this->packet = packet;
    return pipeline->Build(builder, delta);
}

IPipeline *PostProcessingProcess::GetPipeline() {
    return pipeline;
}

PostProcessingResources PostProcessingProcess::GetResources() {
    return resources;
}

CameraPacket *PostProcessingProcess::GetPacket() {
    return packet;
}
