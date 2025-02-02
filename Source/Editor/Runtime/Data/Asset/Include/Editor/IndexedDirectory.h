//
// Created by Dev on 3/22/2018.
//
#pragma once

#include <Core/StringView.h>
#include <Core/Array.h>
#include "IIndexedDirectory.h"

namespace Djinn::Editor {
    class DJINN_EXPORT_EDITOR_DATA_ASSET IndexedDirectory : public IIndexedDirectory {
    public:
        DJINN_COM_CLASS();

        IndexedDirectory(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this directory
        /// \param name
        /// \return
        COM::Result Initialize(const Core::StringView& name);

        /// Overrides
        Core::StringView GetName() override;
        void Register(const Base::PathID &path) override;
        void Register(IIndexedDirectory *directory) override;
        Core::ArrayView<Base::PathID> GetAssets() override;
        Core::ArrayView<IIndexedDirectory*> GetDirectories() override;

    private:
        Core::String name;
        Core::Array<Base::PathID> files;
        Core::Array<IIndexedDirectory*> directories;
    };
}