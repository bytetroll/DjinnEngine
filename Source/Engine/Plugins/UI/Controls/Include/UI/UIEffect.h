//
// Created by Dev on 3/26/2018.
//
#pragma once

#include <Core/Memory.h>
#include <Core/Annotation.h>

namespace Djinn::UI {
    enum class UIEffectType {
        eBubble,
        eClick,
        eWiggle,
        eRotate,
        eStretch
    };

    struct UIEffect {
        /// Create click event
        /// \param duration
        /// \return
        static UIEffect Click(double duration, float strength = 1.0f) {
            UIEffect effect;
            effect.type = UIEffectType::eClick;
            effect.duration = duration;
            effect.data.strength = strength;
            return effect;
        }

        /// Create bubble event
        /// \param duration
        /// \return
        static UIEffect Bubble(double duration, float strength = 1.0f) {
            UIEffect effect;
            effect.type = UIEffectType::eBubble;
            effect.duration = duration;
            effect.data.strength = strength;
            return effect;
        }

        /// Create Wiggle event
        /// \param duration
        /// \return
        static UIEffect Wiggle(double duration, float strength = 1.0f) {
            UIEffect effect;
            effect.type = UIEffectType::eWiggle;
            effect.duration = duration;
            effect.data.strength = strength;
            return effect;
        }

        /// Create Stretch event
        /// \param duration
        /// \return
        static UIEffect Stretch(double duration, float strength = 1.0f) {
            UIEffect effect;
            effect.type = UIEffectType::eStretch;
            effect.duration = duration;
            effect.data.strength = strength;
            return effect;
        }

        /// Create Rotate event
        /// \param duration
        /// \return
        static UIEffect Rotate(double duration, float speed = 0.45f) {
            UIEffect effect;
            effect.type = UIEffectType::eRotate;
            effect.duration = duration;
            effect.data.rotateSpeed = speed;
            return effect;
        }

        /// Type of this effect
        UIEffectType type;

        /// Total duration of this effect
        double duration = 0.0f;

        /// Associated data
        ClassAttr(Ignore)
        union Data {
            Data() = default;

            /// Copy
            /// \param other
            Data(const UIEffect &other) {
                Core::Memory::Copy(this, &other, sizeof(UIEffect));
            }

            /// Data
            float rotateSpeed;
            float strength;
        } data;
    };
}
