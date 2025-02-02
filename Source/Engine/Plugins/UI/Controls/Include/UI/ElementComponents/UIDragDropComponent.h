//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>
#include <Core/Template/Limits.h>
#include <Core/BitSet.h>
#include <Math/Vector2.h>
#include <Math/Vector2I.h>
#include <Game/ActorEvent.h>

namespace Djinn::UI {
    class IUITransformComponent;

    /// Delegates
    using UIDragDropMovedDelegate = Core::Delegate<void()>;

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIDragDropComponent : public IUIElementComponent {
    public:
        DJINN_COM_CLASS();

        UIDragDropComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        void StaticInitialize() override;

    public:
        /// Set the masked faces
        /// \param mask
        void SetMask(const Math::Vector2& mask) {
            dragMask = mask;
        }

        /// Get the masked faces
        /// \return
        const Math::Vector2& GetMask() const {
            return dragMask;
        }

        /// Set the masked faces
        /// \param mask
        void SetRestricted(bool isRestricted) {
            this->restrictedBounds = isRestricted;
        }

        /// Get the masked faces
        /// \return
        bool IsRestricted() const {
            return restrictedBounds;
        }

        /// Set the movement transform
        /// \param transform
        void SetMovementTransform(IUITransformComponent* transform);

        /// Get the movement transform
        /// \return
        IUITransformComponent* GetMovementTransform() const {
            return movementTransform;
        }

        /// Events
        Game::ActorEvent<UIDragDropMovedDelegate> movedEvents;

    private:
        /// Target transform
        Core::WeakPtr<IUITransformComponent> movementTransform;

        /// Drag mask
        Math::Vector2 dragMask = {1, 1};

        /// Is the dragging restricted in the current bounds?
        bool restrictedBounds = false;

        /// Base drag position
        Math::Vector2 baseDragOffset;

        /// Is this component currently being dragged?
        bool isDragging = false;
    };
}