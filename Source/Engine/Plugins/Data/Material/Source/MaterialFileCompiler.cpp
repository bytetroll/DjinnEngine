//
// Created by Dev on 3/16/2018.
//

#include <Data/MaterialFileCompiler.h>
#include <Data/MaterialConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

MaterialFileCompiler::MaterialFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result MaterialFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result MaterialFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    MaterialConfiguration config;

    // Compile
    return host->CompileAsset(path, &config, out);
}

