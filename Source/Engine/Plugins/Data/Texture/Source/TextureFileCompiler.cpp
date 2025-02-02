//
// Created by Dev on 3/16/2018.
//

#include <Data/TextureFileCompiler.h>
#include <Data/TextureConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

TextureFileCompiler::TextureFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result TextureFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result TextureFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    TextureConfiguration config;
    config.compressionMode = TextureCompressionMode::eBC7;

    // Determine format
    if (extension == ".jpg") {
        config.sourceFormat = TextureSourceFormat::eJpg;
    } else if (extension == ".png") {
        config.sourceFormat = TextureSourceFormat::ePng;
    } else {
        ErrorLog(this).Write("Unexpected texture extension");
        return COM::kError;
    }

    // Compile
    return host->CompileAsset(path, &config, out);
}

