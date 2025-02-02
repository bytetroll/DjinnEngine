//
// Created by Dev on 7/13/2018.
//
#pragma once

#include <Game/ISceneComponent.h>

namespace Djinn::Editor {
    class RuntimeComponent : public Game::ISceneComponent {
    public:
        DJINN_COM_CLASS();

        RuntimeComponent(Host::IRegistry*registry, IUnknown *outer, COM::IInstanceHost* host);

    public:
        /// Get the underlying component id
        /// \return
        const COM::ClassID& GetComponentID() const {
            return componentID;
        }

        /// Set the underlying component id
        /// \param cid
        void SetComponentID(const COM::ClassID& cid) {
            componentID = cid;
        }

    private:
        COM::ClassID componentID;
    };
}
