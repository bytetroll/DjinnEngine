//
// Created by Dev on 2/25/2018.
//

#include <Graphics/RootProcess.h>
#include <Graphics/Pipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Graphics);

RootProcess::RootProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IProcess(registry, outer, host) {

}

COM::Result RootProcess::Initialize() {
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to initialize root process, could not create pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

IPipeline *RootProcess::GetPipeline() {
    return pipeline;
}


COM::Result RootProcess::Build(Graphics::PassBuilder builder, double delta) {
    return pipeline->Build(builder, delta);
}
