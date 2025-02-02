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