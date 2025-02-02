//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <Core/Array.h>
#include <Data/WorldFeature.h>
#include <Core/HashMap.h>
#include <Base/Pipeline.h>
#include "IWorldHost.h"

namespace Djinn::Game {
    class WorldHost : public IWorldHost {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        WorldHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \param config
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result CreateFeature(Data::WorldFeatureID id, IWorldFeature **out) override;
        IWorldFeature *GetFeature(Data::WorldFeatureID id) override;
        COM::Result CreateWorld(Data::WorldConfig *config, IWorld** out) override;
        COM::Result LoadWorld(Data::IWorldAsset *asset, IWorld **out) override;
        COM::Result SaveWorld(Data::IWorldAsset *asset, IWorld *world) override;
        COM::Result Register(IWorld *world) override;
        COM::Result Deregister(IWorld *world) override;
        COM::Result EnumerateWorlds(UInt32 &count, IWorld **&worlds) override;
        COM::Result Register(IWorldFeature *feature) override;
        COM::Result Deregister(IWorldFeature *feature) override;
        Base::IPipeline *GetPipeline() override;

    private:
        COM::Result InitializePackage(IWorld* world, const COM::ClassID& cid);

    private:
        Core::Array<IWorldFeature*> features;
        Core::HashMap<Data::WorldFeatureID, IWorldFeature*> featureMap;
        Core::Array<IWorld*> worlds;
        Data::WorldConfig* config = nullptr;

        Base::Pipeline* pipeline;
    };
}