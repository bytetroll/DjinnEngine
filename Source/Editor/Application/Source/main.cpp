//
// Created by Dev on 6/27/2018.
//

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
