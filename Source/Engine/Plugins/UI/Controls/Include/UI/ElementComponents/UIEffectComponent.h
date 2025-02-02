//
// Created by Dev on 8/20/2018.
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