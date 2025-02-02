//
// Created by Dev on 2/6/2018.
//

#include <Data/AssetCompilerRegistry.h>
#include <Data/IAssetCompiler.h>

DJINN_NS2(Data);

AssetCompilerRegistry::AssetCompilerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompilerRegistry(registry, outer, host) {

}

COM::Result AssetCompilerRegistry::AddCompiler(IAssetCompiler *compiler) {
    compilers[compiler->GetClassID()] = compiler;
    return COM::kOK;
}

COM::Result AssetCompilerRegistry::AddCompiler(const COM::ClassID &id, IAssetCompiler *compiler) {
    if (!compiler) {
        return COM::kInvalidParameter;
    }

    compilers[compiler->GetClassID()] = compiler;
    mapped[id] = compiler;
    return COM::kOK;
}

COM::Result AssetCompilerRegistry::AddCompiler(const COM::ClassID &id, const COM::ClassID &compilerID) {
    auto&& it = compilers.Find(compilerID);
    if (it == compilers.end()) {
        return COM::kError;
    }

    mapped[id] = (*it).second;
    return COM::kOK;
}

COM::Result AssetCompilerRegistry::RemoveCompiler(const COM::ClassID &id) {
    mapped.Remove(id);
    return COM::kOK;
}

COM::Result AssetCompilerRegistry::GetCompiler(const COM::ClassID &id, IAssetCompiler **out) {
    auto&& it = mapped.Find(id);
    if (it == mapped.End()) {
        return kNoCompiler;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result AssetCompilerRegistry::GetCompilerWithID(const COM::ClassID &id, IAssetCompiler **out) {
    auto&& it = compilers.Find(id);
    if (it == compilers.End()) {
        return kNoCompiler;
    }

    *out = (*it).second;
    return COM::kOK;
}

