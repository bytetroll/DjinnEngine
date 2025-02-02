//
// Created by Dev on 3/22/2018.
//

#include <Editor/AssetRegistry.h>
#include <Editor/IndexedDirectory.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Editor);

AssetRegistry::AssetRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetRegistry(registry, outer, host) {

}

COM::Result AssetRegistry::Initialize() {
    if (!registry->CreateClass(this, &directory) || !directory->Initialize("")) {
        ErrorLog(this).Write("Failed to create base directory");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result AssetRegistry::Register(const Base::PathID &id) {
    assets.Add(id);
    return COM::kOK;
}

IIndexedDirectory *AssetRegistry::GetDirectory() {
    return directory;
}
