//
// Created by Dev on 3/16/2018.
//

#include <Data/FontFileCompiler.h>
#include <Data/FontConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

FontFileCompiler::FontFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result FontFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result FontFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    FontConfiguration config;

    // Determine format
    if (extension == ".otf") {
        config.sourceFormat = FontSourceFormat ::eOTF;
    } else if (extension == ".ttf") {
        config.sourceFormat = FontSourceFormat ::eTTF;
    } else {
        ErrorLog(this).Write("Unexpected font extension");
        return COM::kError;
    }

    // Compile
    return host->CompileAsset(path, &config, out);
}

