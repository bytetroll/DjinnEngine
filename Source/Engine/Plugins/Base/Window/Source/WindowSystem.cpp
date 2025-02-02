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
 on 2/25/2018.
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
