//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 2/6/2018.
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

