//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/HashMap.h>
#include <Core/Array.h>
#include "WorldFeature.h"

namespace Djinn::Data {
    class IWorldSetting;

    class DJINN_EXPORT_PLUGINS_DATA_WORLD WorldConfig : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        WorldConfig(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Get a setting, created if not present
        /// \return
        IWorldSetting *Get(COM::ClassID cid);

        /// Get a setting, created if not present
        /// \return
        template<typename T>
        T *Get() {
            return static_cast<T *>(Get(T::kCID));
        }

        /// Add a feature to this configuration
        /// \param id
        void AddFeature(WorldFeatureID id) {
            features.Add(id);
        }

        /// Settings
        Core::HashMap<COM::ClassID, IWorldSetting *> settings;

        /// Features
        Core::Array<WorldFeatureID> features;
    };
}