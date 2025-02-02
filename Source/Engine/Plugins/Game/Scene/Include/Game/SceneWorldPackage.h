//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <Game/IWorldPackage.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE SceneWorldPackage : public IWorldPackage {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        SceneWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result Initialize(IWorld *world) override;
        COM::Result Initialize(IScene *scene) override;
        COM::Result Serialize(Serialization::Archive &archive) override;
        COM::Result Deserialize(Serialization::Archive &archive) override;
    };
}