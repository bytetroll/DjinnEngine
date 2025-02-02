//
// Created by Dev on 3/16/2018.
//

#include <Data/MeshFileCompiler.h>
#include <Data/MeshConfiguration.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Platform/FileSystem.h>

DJINN_NS2(Data);

MeshFileCompiler::MeshFileCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileAssetCompiler(registry, outer, host) {

}

COM::Result MeshFileCompiler::Initialize(IAssetHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result MeshFileCompiler::Compile(const Base::PathID &path, const Core::String &absolute, const Core::String &extension, IAsset **out) {
    // Base configuration
    MeshConfiguration config;
    config.includePath = Platform::FileSystem::GetDirectory(absolute);
    config.includePath.Replace('\\', '/');

    // Determine format
    if (extension == ".obj") {
        config.format = MeshSourceFormat ::eObj;
    } else {
        ErrorLog(this).Write("Unexpected mesh extension");
        return COM::kError;
    }

    // Compile
    return host->CompileAsset(path, &config, out);
}

