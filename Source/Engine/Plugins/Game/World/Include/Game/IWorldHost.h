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