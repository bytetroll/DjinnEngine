//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>
#include <Core/Template/Limits.h>
#include <Core/BitSet.h>
#include <Math/Vector2.h>
#include <Math/Vector2I.h>

namespace Djinn::UI {
    class IUITransformComponent;

    enum class UIResizeFace {
        eTopLeft = 1,
        eTop = static_cast<int>(eTopLeft) << 1,
        eTopRight = static_cast<int>(eTop) << 1,
        eRight = static_cast<int>(eTopRight) << 1,
        eBottomRight = static_cast<int>(eRight) << 1,
        eBottom = static_cast<int>(eBottomRight) << 1,
        eBottomLeft = static_cast<int>(eBottom) << 1,
        eLeft = static_cast<int>(eBottomLeft) << 1,
        eAll = Core::LimitsOf<int>::kMax
    };
    DJINN_CORE_BITSET(UIResizeFace);

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIResizeComponent : public IUIElementComponent {
    public:
        DJINN_COM_CLASS();

        UIResizeComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        void StaticInitialize() override;

    public:
        /// Set the masked faces
        /// \param mask
        void SetFaces(UIResizeFaceSet mask) {
            faces = mask;
        }

        /// Get the masked faces
        /// \return
        UIResizeFaceSet GetFaces() const {
            return faces;
        }

        /// Set the resize transform
        /// \param transform
        void SetResizeTransform(IUITransformComponent* transform);

        /// Get the resize transform
        /// \return
        IUITransformComponent* GetResizeTransform() const {
            return resizeTransform;
        }

    private:
        /// Target transform
        Core::WeakPtr<IUITransformComponent> resizeTransform;

        /// Base drag position
        Math::Vector2 baseDragOffset;

        /// Drag direction
        Math::Vector2 marginMask = {0, 0};
        Math::Vector2 desiredSizeMask = {0, 0};

        /// Face set
        UIResizeFaceSet faces = UIResizeFace::eAll;

        /// Distance treshold
        float treshold = 10;

        /// Is this component currently being resized?
        bool isResizing = false;
    };
}