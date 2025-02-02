//
// Created by Dev on 4/8/2018.
//

#include <Graphics/OutputHost.h>
#include <Graphics/OutputPass.h>
#include <Graphics/Output.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Graphics);

OutputHost::OutputHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IOutputHost(registry, outer, host) {

}

COM::Result OutputHost::Initialize(OutputPass *pass) {
    this->pass = pass;

    return COM::kOK;
}

COM::Result OutputHost::SetOutput(IOutput *output) {
    return pass->SetOutput(output);
}

COM::Result OutputHost::Register(IOutput *output) {
    return pass->Register(output);
}

COM::Result OutputHost::Deregister(IOutput *output) {
    return pass->Deregister(output);
}
