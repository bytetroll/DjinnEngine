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
 on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>
#include <UI/UIEffect.h>
#include <Core/FixedArray.h>
#include <Game/ActorCallback.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIEffectComponent : public IUIElementComponent {
    public:
        DJINN_COM_CLASS();

        UIEffectComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        void StaticInitialize() override;

        /// Update
        /// \param delta
        void Update(double delta);

    public:
        /// Set an effect, clears all previous events
        /// \param effect
        void Set(const UIEffect& effect) {
            effectDatas.Clear();
            effectDatas.Add({effect, 0});
            callback.Register(this);
        }

        /// Add an effect
        /// \param effect
        void Add(const UIEffect& effect) {
            effectDatas.Add({effect, 0});
            callback.Register(this);
        }

        /// Clear all effects
        void Clear() {
            effectDatas.Clear();
            callback.Deregister();
        }

    public:
        struct EffectData {
            UIEffect effect;
            double elapsed = 0;
        };

        /// Callback
        Game::ActorCallback callback;

        /// All effects
        Core::FixedArray8<EffectData> effectDatas;
    };
}