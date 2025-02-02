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

#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Game/ISceneComponent.h>
#include <Core/Array.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE ITransformComponent : public ISceneComponent {
    public:
        DJINN_COM_INTERFACE();

        ITransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneComponent(registry, outer, host) {

        }

        /// Update the transform
        void UpdateTransform(bool force = false);

        /// Clean the transform
        /// Mark all states as clean
        void Clean();

    public:
        /// Update this transform
        virtual void Update() = 0;

    public:
        /// Set the parent transform
        /// \param parent
        void SetParent(ITransformComponent* parent);

        /// Set the relative position
        /// \param position
        void SetPosition(const Math::Vector3& position) {
            this->position = position;
            MarkAsDirty();
        }

        /// Set the relative scale
        /// \param scale
        void SetScale(const Math::Vector3& scale) {
            this->scale = scale;
            MarkAsDirty();
        }

        /// Set the relative rotation
        /// \param rotation
        void SetRotation(const Math::Quaternion& rotation) {
            this->rotation = rotation;
            MarkAsDirty();
        }

        void SetUserTransform(const Math::Matrix4& user) {
            this->userTransform = user;
            MarkAsDirty();
        }

        /// Get the relative position
        /// \return
        const Math::Vector3& GetPosition() const {
            return position;
        }

        /// Get the relative scale
        /// \return
        const Math::Vector3& GetScale() const {
            return scale;
        }

        /// Get the relative rotation
        /// \return
        const Math::Quaternion& GetRotation() const {
            return rotation;
        }

        /// Get the user transform
        /// \return
        const Math::Matrix4& GetUserTransform() const {
            return userTransform;
        }

        /// Get the world transform
        /// \return
        const Math::Matrix4& GetWorldTransform() const {
            return worldTransform;
        }

        /// Get the parent transform
        /// \return
        ITransformComponent* GetParent() const {
            return parent;
        }

        /// Get forward vector
        /// \return
        Math::Vector3 GetForward() {
            return (rotation * Math::Vector3(0, 0, 1)).Normalized();
        }

        /// Get right vector
        /// \return
        Math::Vector3 GetRight() {
            return (rotation * Math::Vector3(1, 0, 0)).Normalized();
        }

        /// Get up vector
        /// \return
        Math::Vector3 GetUp() {
            return (rotation * Math::Vector3(0, 1, 0)).Normalized();
        }

        /// Mark this transform as dirty
        void MarkAsDirty(bool force = false) {
            if (isDirty && !force) {
                return;
            }

            // Mark as dirty
            isDirty = true;

            // Propagate
            auto self = this;
            while (self) {
                // Immediate parent?
                if (self->parent && !self->isAdded) {
                    self->parent->isChildDirty = true;
                    self->parent->dirtyChildren.Add(self->GetWeakPtr());
                    self->isAdded = true;
                }

                // ...
                self = self->parent;
            }
        }

        /// Mark this transform as clean
        void MarkAsClean() {
            isDirty = false;
            isChildDirty = false;
        }

    protected:
        /// Is this transform dirty?
        bool isDirty = true;

        /// Is a child dirty?
        bool isChildDirty = true;

        bool isAdded = false;

    protected:
        /// Parent
        Core::WeakPtr<ITransformComponent> parent;

        /// Relative position
        Math::Vector3 position{0, 0, 0};

        /// Relative scale
        Math::Vector3 scale{1, 1, 1};

        /// Relative rotation
        Math::Quaternion rotation = Math::Quaternion::Identity();

    protected:
        /// User transform
        Math::Matrix4 userTransform = Math::Matrix4::Identity();

        /// Final transform, generated
        Math::Matrix4 worldTransform = Math::Matrix4::Identity();

        /// All children
        Core::Array<Core::WeakPtr<ITransformComponent>> children;

        /// Dirty children
        Core::Array<Core::WeakPtr<ITransformComponent>> dirtyChildren;
    };
}