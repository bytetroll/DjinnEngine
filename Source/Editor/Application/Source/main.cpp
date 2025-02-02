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

#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Core/Timer.h>
#include <Editor/ILevelHost.h>
#include <Editor/Elements/IMainLayout.h>

DJINN_NS2(Editor);

void Startup(Base::Application &app) {
    Core::Timer timer;

    // Load all engine plugins
    if (COM::CheckedResult result = app.GetRegistry()->LoadPlugins()) {
        ErrorLog(app.GetRegistry()).Write("Automatic plugin loading failed");
        return;
    }

    // Load all editor plugins
    if (COM::CheckedResult result = app.GetRegistry()->LoadPlugins("Editor.*.dll")) {
        ErrorLog(app.GetRegistry()).Write("Automatic plugin loading failed");
        return;
    }

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");

    // Get layout
    IMainLayout* mainLayout;
    if (!app.GetRegistry()->GetPipeline()->GetInterface(&mainLayout)) {
        ErrorLog(app.GetRegistry()).Write("Expected level layout to be installed");
        return;
    }

    // Create default level
    {
        ILevelHost* host;
        if (!app.GetRegistry()->GetPipeline()->GetInterface(&host)) {
            ErrorLog(app.GetRegistry()).Write("Expected world worldHost to be installed");
            return;
        }

        // Create level
        ILevel* level;
        if (!host->CreateLevel(&level)) {
            return;
        }

        // Initialize layout
        CheckVoid(mainLayout->AddLevel(level));
    }
}

int main() {
    if (COM::CheckedResult result = Base::Launch(Base::ApplicationInfo().Name("Editor"), Bindg(Startup))) {
        return 1;
    }
    return 0;
}
