//
// Created by Dev on 5/5/2018.
//

#include <Data/UIMLFileCompiler.h>
#include <Data/UIMLConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

UIMLFileCompiler::UIMLFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result UIMLFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result UIMLFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    UIMLConfiguration config;

    // Compile
    return host->CompileAsset(path, &config, out);
}
