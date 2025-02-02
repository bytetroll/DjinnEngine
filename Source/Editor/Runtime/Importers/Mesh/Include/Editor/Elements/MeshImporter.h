//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/IControl.h>

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Editor {
    class MeshImporter : public UI::IControl {
    public:
        DJINN_COM_CLASS();

        MeshImporter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

        /// Events
        void Created() override;

    private:
        Base::IFileSystem *fs;
    };
}
