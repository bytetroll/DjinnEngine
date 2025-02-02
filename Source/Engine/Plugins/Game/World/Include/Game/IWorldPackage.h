//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Serialization/Archive.h>

namespace Djinn::Game {
    class IWorld;
    class IScene;

    class IWorldPackage : public COM::TUnknown<IWorldPackage> {
    public:
        DJINN_COM_INTERFACE();

        IWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this package onto a given world
        /// \param world
        /// \return
        virtual COM::Result Initialize(IWorld* world) { return COM::kOK; };

        /// Initialize this package onto a given scene
        /// \param scene
        /// \return
        virtual COM::Result Initialize(IScene* scene) { return COM::kOK; };

        /// Serialize into archive
        /// \param object
        /// \return
        virtual COM::Result Serialize(Serialization::Archive &archive) = 0;

        /// Inplace deserialization from archive
        /// \param object
        /// \return
        virtual COM::Result Deserialize(Serialization::Archive& archive) = 0;
    };
}