//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class ILevel;
    class ILevelAsset;

    class ILevelHost : public COM::TUnknown<ILevelHost> {
    public:
        DJINN_COM_INTERFACE();

        ILevelHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Create a new level
        /// \param out
        /// \return
        virtual COM::Result CreateLevel(ILevel **out) = 0;

        /// Save the level to an asset
        /// \param asset
        /// \return
        virtual COM::Result SaveLevel(ILevelAsset** asset) = 0;

        /// Load the level from an asset
        /// \param asset
        /// \return
        virtual COM::Result LoadLevel(ILevelAsset* asset) = 0;
    };
}