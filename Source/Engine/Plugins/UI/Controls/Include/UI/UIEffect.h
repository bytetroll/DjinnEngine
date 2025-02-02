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
 on 3/26/2018.
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
