//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <Core/HashMap.h>
#include "IAssetCompilerRegistry.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_ASSET AssetCompilerRegistry : public IAssetCompilerRegistry {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        AssetCompilerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
                COM::Result AddCompiler(IAssetCompiler *compiler) override;
        COM::Result AddCompiler(const COM::ClassID &id, IAssetCompiler *compiler) override;
        COM::Result AddCompiler(const COM::ClassID &id, const COM::ClassID &compilerID) override;
        COM::Result RemoveCompiler(const COM::ClassID &id) override;
        COM::Result GetCompiler(const COM::ClassID &id, IAssetCompiler **out) override;
        COM::Result GetCompilerWithID(const COM::ClassID &id, IAssetCompiler **out) override;

    private:
        Core::HashMap<COM::ClassID, IAssetCompiler*> mapped;
        Core::HashMap<COM::ClassID, IAssetCompiler*> compilers;
    };
}