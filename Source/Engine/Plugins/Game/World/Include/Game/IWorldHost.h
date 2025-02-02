//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Data/WorldFeature.h>

namespace Djinn::Data {
    class IWorldAsset;
    class WorldConfig;
}

namespace Djinn::Base {
    class IPipeline;
}

namespace Djinn::Game {
    class IWorld;
    class IWorldPackage;
    class IWorldFeature;

    /// Default world feature
    DJINN_WORLD_FEATURE(DefaultFeature);

    class IWorldHost : public COM::IUnknown {
    public:
        DJINN_COM_INTERFACE();

        IWorldHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

        }

        /// Create a feature with given id
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result CreateFeature(Data::WorldFeatureID id, IWorldFeature** out) = 0;

        /// Get a world feature
        /// \param id
        /// \return
        virtual IWorldFeature* GetFeature(Data::WorldFeatureID id) = 0;

        /// Create a world based on the given configuration
        /// \param config
        /// \return
        virtual COM::Result CreateWorld(Data::WorldConfig* config, IWorld** out) = 0;

        /// Load a world from an asset
        /// \param stream
        /// \param out
        /// \return
        virtual COM::Result LoadWorld(Data::IWorldAsset* asset, IWorld** out) = 0;

        /// Save a world to an asset
        /// \param stream
        /// \param world
        /// \return
        virtual COM::Result SaveWorld(Data::IWorldAsset* asset, IWorld* world) = 0;

        /// Register a world
        /// \param world
        /// \return
        virtual COM::Result Register(IWorld* world) = 0;

        /// Deregister a world
        /// \param world
        /// \return
        virtual COM::Result Deregister(IWorld* world) = 0;

        /// Register a feature
        /// \param feature
        /// \return
        virtual COM::Result Register(IWorldFeature* feature) = 0;

        /// Deregister a feature
        /// \param feature
        /// \return
        virtual COM::Result Deregister(IWorldFeature* feature) = 0;

        /// Enumerate all worlds
        /// \param count
        /// \param worlds
        /// \return
        virtual COM::Result EnumerateWorlds(UInt32& count, IWorld**& worlds) = 0;

        /// Get the world execution pipeline
        /// \return
        virtual Base::IPipeline* GetPipeline() = 0;
    };
}