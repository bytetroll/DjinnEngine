//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <Game/IWorldPackage.h>

namespace Djinn::Editor {
    class DJINN_EXPORT_EDITOR_VIEWPORT ViewportWorldPackage : public Game::IWorldPackage {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        ViewportWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result Initialize(Game::IScene *scene) override;
        COM::Result Serialize(Serialization::Archive &archive) override;
        COM::Result Deserialize(Serialization::Archive &archive) override;
    };
}