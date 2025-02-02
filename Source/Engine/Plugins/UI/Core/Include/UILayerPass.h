//
// Created by Dev on 1/9/2019.
//
#pragma once

#include <Core/Types.h>

namespace Djinn::UI {
    class IUIRenderComponent;

    struct UILayerController {
        UInt32 offset = 0;
    };

    struct UILayerPass {
        UILayerPass(UILayerController &controller) : controller(controller) {

        }

        /// Get current offset
        /// \return
        UInt32 GetOffset() const {
            return controller.offset;
        }

        /// Push next pass
        /// \return
        UILayerPass Next() {
            controller.offset++;
            return {controller};
        }

    private:
        UILayerController &controller;
    };
}