//
// Created by Dev on 7/27/2018.
//

#include <Editor/IndexedDirectory.h>

DJINN_NS2(Editor);

IndexedDirectory::IndexedDirectory(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IIndexedDirectory(registry, outer, host) {

}

COM::Result IndexedDirectory::Initialize(const Core::StringView &name) {
    this->name = name.ToString();

    // OK
    return COM::kOK;
}

Core::StringView IndexedDirectory::GetName() {
    return name;
}

void IndexedDirectory::Register(const Base::PathID &path) {
    files.Add(path);
}

void IndexedDirectory::Register(IIndexedDirectory *directory) {
    directories.Add(directory);
}

Core::ArrayView<Base::PathID> IndexedDirectory::GetAssets() {
    return files;
}

Core::ArrayView<IIndexedDirectory*> IndexedDirectory::GetDirectories() {
    return directories;
}
