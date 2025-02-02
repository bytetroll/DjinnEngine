//
// Created by Dev on 3/13/2018.
//
#pragma once

#include "UIInputAction.h"
#include <Core/Array.h>

namespace Djinn::UI {
    class UIInputActionBuffer {
    public:
        /// Add action
        /// \param action
        void Add(const UIInputAction& action) {
            actions.Add(action);
        }

        /// Flush all actions
        void Flush() {
            actions.Clear(false);
        }

        /// Get actions
        /// \return
        const Core::Array<UIInputAction>& GetActions() const {
            return actions;
        }

    private:
        Core::Array<UIInputAction> actions;
    };
}