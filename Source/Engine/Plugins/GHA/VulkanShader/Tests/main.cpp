//
// Created by Dev on 11/30/2017.
//

#include <Base/Application.h>
#include <Core/Timer.h>
#include <Base/DefaultLogs.h>
#include <Data/IAssetHost.h>
#include <Platform/FileStream.h>
#include <Data/ShaderAsset.h>
#include <Data/ShaderConfiguration.h>

DJINN_NS();

void _main(Base::Application &app) {
    Core::Timer timer;

    // Load plugins
    Always(app.GetRegistry()->LoadPlugins());

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");

    // Get asset worldHost
    Data::IAssetHost* host;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&host));

    Platform::FileStream stream(Platform::FileMode::eRead, "../Data/texarray.dsf");
    Always(stream.IsGood());

    // Compile
    Data::ShaderConfiguration config;
    Data::ShaderAsset* shader;
    Always(host->CompileAsset(&stream, &config, &shader));
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("UI Graphics Test"), Bindg(_main));
    std::cin.ignore();
    return 0;
}