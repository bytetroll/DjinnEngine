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