//
// Created by Dev on 5/5/2018.
//

#include <Base/Application.h>
#include <Data/IAssetHost.h>
#include <Data/IUIMLAsset.h>
#include <Data/UIMLConfiguration.h>
#include <Platform/FileStream.h>

DJINN_NS();

void _main(Base::Application &app) {
    // Load plugins
    Always(app.GetRegistry()->LoadPlugin("Plugins.Base.FileSystem"));
    Always(app.GetRegistry()->LoadPlugin("Plugins.Data.Asset"));
    Always(app.GetRegistry()->LoadPlugin("Plugins.Data.UIML"));

    // ...
    Data::IAssetHost* host;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&host));

    // Load test
    Platform::FileStream stream(Platform::FileMode::eRead, "../misc.uiml");
    Always(stream.IsGood());

    Data::IUIMLAsset *asset;
    Data::UIMLConfiguration config;
    host->CompileAsset<Data::IUIMLAsset>(&stream, &config, &asset);
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("UIML Test"), Bindg(_main));
    return 0;
}