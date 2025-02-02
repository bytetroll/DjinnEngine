//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <Game/IWorldPackage.h>

namespace Djinn::UI {
class DJINN_EXPORT_PLUGINS_UI_DESKTOP DesktopUIWorldPackage : public Game::IWorldPackage {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        DesktopUIWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Initialize(Game::IWorld *world) override;
        virtual COM::Result Serialize(Serialization::Archive &archive) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive) override;
    };
}