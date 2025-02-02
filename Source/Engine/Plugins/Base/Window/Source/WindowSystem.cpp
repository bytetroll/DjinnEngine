//
// Created by Dev on 2/25/2018.
//

#include <Base/WindowSystem.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Base);

WindowSystem::WindowSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWindowSystem(registry, outer, host) {

}

COM::Result WindowSystem::Initialize(const Platform::WindowDesc &desc, bool focus) {
    Check(window.Initialize(desc));

    // Focus?
    if (focus) {
        window.Show();
        window.SetForegroundWindow();
        window.SetFocus();
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

Platform::Window *WindowSystem::GetWindow() {
    return &window;
}

COM::Result WindowSystem::Run() {
    return COM::kOK;
}

COM::Result WindowSystem::Configure(PipelineBuilder& builder) {
    return COM::kOK;
}
