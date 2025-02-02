//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 2/28/2018.
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