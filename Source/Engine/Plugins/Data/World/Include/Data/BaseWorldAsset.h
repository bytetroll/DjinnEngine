//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "IWorldAsset.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_WORLD BaseWorldAsset : public IWorldAsset {
    public:
        DJINN_COM_CLASS();

        BaseWorldAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldAsset(registry, outer, host) {

        }

        /// Overrides
        virtual WorldConfig *GetConfig() override;

        /// Configuration
        WorldConfig* config = nullptr;
    };
}