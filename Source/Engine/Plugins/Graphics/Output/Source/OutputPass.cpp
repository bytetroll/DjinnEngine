//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 4/8/2018.
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
