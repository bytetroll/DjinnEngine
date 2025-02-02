//
// Created by Dev on 4/8/2018.
//

#include <Graphics/OutputPass.h>
#include <Graphics/Output.h>
#include <Graphics/IGraphicsSystem.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <GHA/ISwapchain.h>

DJINN_NS2(Graphics);

OutputPass::OutputPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IOutputPass(registry, outer, host) {

}

COM::Result OutputPass::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("Expected graphics system to be installed");
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result OutputPass::Preprocess(PassBuilder builder) {
    // Configure all outputs
    for (auto output : outputs) {
        if (!output->Configure(builder)) {
            ErrorLog(output).Write("Configuration failed");
        }
    }

    // OK
    return COM::kOK;
}

COM::Result OutputPass::Build(PassBuilder builder, double delta) {
    // Must have output attached
    if (!output) {
        return COM::kOK;
    }

    // Copy to output
    builder.CopyTexture(output->GetTexture(builder), builder.AddTexture(system->GetSwapchain()->GetTexture()));

    // OK
    return COM::kOK;
}

COM::Result OutputPass::Configure() {
    return COM::kOK;
}

COM::Result OutputPass::SetOutput(IOutput *output) {
    this->output = output;
    return COM::kOK;
}

COM::Result OutputPass::Register(IOutput *output) {
    outputs.Add(output);
    return COM::kOK;
}

COM::Result OutputPass::Deregister(IOutput *output) {
    return outputs.RemoveValue(output) ? COM::kOK : COM::kError;
}
