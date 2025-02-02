//
// Created by Dev on 3/16/2018.
//

#include <Data/ShaderFileCompiler.h>
#include <Data/ShaderConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

ShaderFileCompiler::ShaderFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result ShaderFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result ShaderFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    ShaderConfiguration config;

    // Compile
    return host->CompileAsset(path, &config, out);
}

