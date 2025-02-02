//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <Math/Vector2.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <UI/IUIElementComponent.h>
#include <UI/UIHorizontalAlignment.h>
#include <UI/UIVerticalAlignment.h>
#include <UI/UIScaling.h>
#include <UI/UIContentMode.h>
#include <UI/UIViewportMode.h>
#include <Game/ActorEvent.h>

namespace Djinn::UI {
    class IUILayer;
    class Style;

    enum class TransformTraceMode {
        eTrace,
        eInput,
    };

    /// Delegate types
    using UITransformUpdatedEvent = Core::Delegate<void()>;

    class DJINN_EXPORT_PLUGINS_UI_CORE IUITransformComponent : public IUIElementComponent {
    public:
        DJINN_COM_INTERFACE();

        IUITransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

        }

        /// Update the layout
        /// \param layer
        /// \param style
        void UpdateLayout(IUILayer *layer);

        /// Overrides
        void StyleEvent(const ElementStyle &style) override;
        void Destroyed() override;

        /// Move all settings to destination
        /// \param dest
        /// \return
        void Move(IUITransformComponent* dest);

    public:
        /// Compute the size
        /// First pass
        /// \return - The desired size
        virtual Math::Vector2 Measure() = 0;

        /// Apply model specific behaviour
        /// Invoked after Measure pass
        virtual void Model() {}

        /// Arrange this transform
        /// Invoked after Model pass
        virtual void Arrange() = 0;

        /// Trace into this component
        /// \param point
        /// \return
        virtual IUITransformComponent *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) = 0;

        /// Find a child by name
        /// \param name
        /// \return
        virtual IUITransformComponent *FindChild(const Core::StringView &name) = 0;

    public:
        /// Events
        Game::ActorEvent<UITransformUpdatedEvent> updated;

    protected:
        /// Invoked when a child is detached
        /// Useful for removing an internal reference
        /// \param child
        /// \return
        virtual bool OnChildDetached(IUITransformComponent* child) = 0;

        /// Invoked when this transform is marked as dirty
        virtual void OnDirty() { };

        /// Invoked during a layer change, each transform handles their own layer
        /// \param layer
        virtual IUILayer *RequestLayer(IUILayer *layer);

    public:
        /// Get absolute position, not relative to parent
        /// \return
        Math::Vector2 GetAbsolutePosition() const {
            return {pixelTransform.w.x, pixelTransform.w.y};
        }

        /// Set the parent reference
        /// \param com
        void SetParentReference(IUITransformComponent *com);

        /// Get parent transform
        /// \return
        IUITransformComponent *GetParent() const {
            return parent;
        }

        /// Set alignment
        /// \param alignment
        void SetAlignment(UIHorizontalAlignment horizontal, UIVerticalAlignment vertical) {
            if (horizontal != horizontalAlignment || vertical != verticalAlignment) {
                horizontalAlignment = horizontal;
                verticalAlignment = vertical;
                MarkAsDirty();
            }
        }

        /// Set horizontal alignment
        /// \param alignment
        void SetHorizontalAlignment(UIHorizontalAlignment alignment) {
            if (alignment != horizontalAlignment) {
                horizontalAlignment = alignment;
                MarkAsDirty();
            }
        }

        /// Set vertical alignment
        /// \param alignment
        void SetVerticalAlignment(UIVerticalAlignment alignment) {
            if (alignment != verticalAlignment) {
                verticalAlignment = alignment;
                MarkAsDirty();
            }
        }

        /// Set scaling
        /// \param scaling
        void SetScaling(UIScaling horizontal, UIScaling vertical) {
            if (horizontal != horizontalScaling || vertical != verticalScaling) {
                horizontalScaling = horizontal;
                verticalScaling = vertical;
                MarkAsDirty();
            }
        }

        /// Set horizontal scaling
        /// \param scaling
        void SetHorizontalScaling(UIScaling scaling) {
            if (scaling != horizontalScaling) {
                horizontalScaling = scaling;
                MarkAsDirty();
            }
        }

        /// Set vertical scaling
        /// \param scaling
        void SetVerticalScaling(UIScaling scaling) {
            if (scaling != verticalScaling) {
                verticalScaling = scaling;
                MarkAsDirty();
            }
        }

        /// Set content mode
        /// \param mode
        void SetContentMode(UIContentMode mode) {
            if (contentMode != mode) {
                contentMode = mode;
                MarkAsDirty();
            }
        }

        /// Set viewport mode
        /// \param mode
        void SetViewportMode(UIViewportMode mode) {
            if (viewportMode != mode) {
                viewportMode = mode;
                MarkAsDirty();
            }
        }

        /// Set margin
        /// \param margin
        void SetMargin(const Math::Vector2 &_margin) {
            if (_margin != margin) {
                margin = _margin;
                MarkAsDirty();
            }
        }

        /// Set desired size
        /// \param desiredSize
        void SetDesiredSize(const Math::Vector2 &desiredSize);

        /// Set the hidden state
        /// \param hidden
        void SetHidden(bool hidden) {
            if (isHidden != hidden)  {
                isHidden = hidden;
                MarkAsDirty();
            }
        }

        /// Set user translation
        /// \return
        void SetUserTranslation(const Math::Vector2 &value) {
            if (userTranslation != value) {
                userTranslation = value;
                MarkAsDirty();
            }
        }

        /// Set user rotation
        /// \return
        void SetUserRotation(const Math::Quaternion &value) {
            if (userRotation != value) {
                userRotation = value;
                MarkAsDirty();
            }
        }

        /// Reset the user transform
        /// \return
        void ResetUserTransform() {
            if (userScale != Math::Vector2(1, 1) || userRotation != Math::Quaternion::Identity() || userTranslation != Math::Vector2(0, 0)) {
                userScale = {1, 1};
                userRotation = Math::Quaternion::Identity();
                userTranslation = {0, 0};
                MarkAsDirty();
            }
        }

        /// Set user scale
        /// \return
        void SetUserScale(const Math::Vector2 &value) {
            if (userScale != value) {
                userScale = value;
                MarkAsDirty();
            }
        }

        /// Set ui transform
        /// \return
        void SetTransform(const Math::Matrix4 &transform);

        /// Set the viewport
        /// \param viewport
        void SetViewport(const Math::Vector4 &viewport);

        /// Set the local viewport
        /// \param viewport
        void SetLocalViewport(const Math::Vector4 &viewport, const Math::Matrix4& transform, const Math::Vector4& parentViewport) {
            SetViewport({
                                Math::Max(viewport.x + transform.w.x, parentViewport.x),
                                Math::Max(viewport.y + transform.w.y, parentViewport.y),
                                Math::Min(viewport.z + transform.w.x, parentViewport.z),
                                Math::Min(viewport.w + transform.w.y, parentViewport.w)
                        });
        }

        /// Get horizontal alignemnt
        /// \return
        UIHorizontalAlignment GetHorizontalAlignment() const {
            return horizontalAlignment;
        }

        /// Get vertical alignment
        /// \return
        UIVerticalAlignment GetVerticalAlignment() const {
            return verticalAlignment;
        }

        /// Get horizontal scaling
        /// \return
        UIScaling GetHorizontalScaling() const {
            return horizontalScaling;
        }

        /// Get vertical scaling
        /// \return
        UIScaling GetVerticalScaling() const {
            return verticalScaling;
        }

        /// Get content mode
        /// \return
        UIContentMode GetContentMode() const {
            return contentMode;
        }

        /// Get viewport mode
        /// \return
        UIViewportMode GetViewportMode() const {
            return viewportMode;
        }

        /// Get margin
        /// \return
        const Math::Vector2 &GetMargin() const {
            return margin;
        }

        /// Get desired size
        /// \return
        const Math::Vector2 &GetDesiredSize() const {
            return desiredSize;
        }

        /// Get minimum desired size
        /// \return
        const Math::Vector2 &GetMinimumDesiredSize() const {
            return desiredSizeMin;
        }

        /// Get maximum desired size
        /// \return
        const Math::Vector2 &GetMaximumDesiredSize() const {
            return desiredSizeMax;
        }

        /// Get the measured size
        /// ! Invalid before the measure transformation pass
        /// \return
        const Math::Vector2 &GetMeasuredSize() const {
            return measuredSize;
        }

        /// Get the pixel transform
        /// \return
        const Math::Matrix4 &GetPixelTransform() const {
            return pixelTransform;
        }

        /// Get the user transform
        /// \return
        const Math::Vector2 &GetUserTranslation() const {
            return userTranslation;
        }

        /// Get the user transform
        /// \return
        const Math::Quaternion &GetUserRotation() const {
            return userRotation;
        }

        /// Get the user transform
        /// \return
        const Math::Vector2 &GetUserScale() const {
            return userScale;
        }

        /// Get the relative depth offset
        /// \return
        Int32 GetDepthOffset() const {
            return zOffset;
        }

        /// Get the absolute depth
        /// \return
        Int32 GetAbsoluteDepth() const {
            return zAbsolute;
        }

        /// Set the depth offset
        /// \param offset
        void SetDepthOffset(Int32 offset) {
            if (zOffset != offset) {
                zOffset = offset;
                MarkAsDirty();
            }
        }

        /// Get the viewport
        /// \return
        Math::Vector4 GetViewport() const {
            return viewport;
        }

        /// Is this transform visible?
        /// \return
        bool IsVisible() const {
            return isVisible;
        }

        /// Is this transform marked as hidden?
        /// \return
        bool IsMarkedAsHidden() const {
            return isHidden;
        }

        /// Mark this transform as dirty
        void MarkAsDirty(bool force = false) {
            if (isDirty && !force) {
                return;
            }

            // Mark as dirty
            isDirty = true;

            // Mark immediate parent as dirty
            if (parent) {
                parent->isDirty = true;
            }

            // Propagate
            auto self = this;
            while (self) {
                // Immediate parent?
                if (self->parent && !self->isAdded) {
                    if (self->parent->IsChildDependent()) {
                        self->parent->isDirty = true;
                    }
                    self->parent->isChildDirty = true;
                    self->parent->dirtyChildren.Add(self->GetWeakPtr());
                    self->isAdded = true;
                }

                // ...
                self = self->parent;
            }

            // Invoke
            OnDirty();
        }

        /// Mark this transform as clean
        void MarkAsClean() {
            isDirty = false;
            isChildDirty = false;
        }

        /// Set the fallthrough state
        /// \param state
        void SetTraceFallthrough(bool state) {
            isTraceFallthrough = state;
        }

        /// Is this transform trace fallthrough?
        /// \return
        bool IsTraceFallthrough() const {
            return isTraceFallthrough;
        }

        /// Does this transform depend on the parent?
        /// \return
        bool IsParentDependent() {
            return verticalAlignment != UIVerticalAlignment::eTop ||
                   horizontalAlignment != UIHorizontalAlignment::eLeft ||
                   verticalScaling == UIScaling::eFill ||
                   horizontalScaling == UIScaling::eFill;
        }

        /// Does this transform depend on its children?
        /// \return
        bool IsChildDependent() {
            return isChildDependent || horizontalScaling == UIScaling::eContent || verticalScaling == UIScaling::eContent;
        }

        void SetChildDependent(bool state) {
            isChildDependent = state;
        }

        /// Set the element for tracing references
        /// \param element
        void SetTraceElement(IUIElement *element);

        /// Get the trace element
        /// \return
        IUIElement *GetTraceElement() const {
            return traceElement ?: GetElement();
        }

        /// Get the layer
        /// \return
        IUILayer *GetLayer() const {
            return layer;
        }

        /// Set the layer
        /// \param layer
        void SetLayer(IUILayer *layer);

    public:
        bool IsPointInside(const Math::Vector2 &point);

    private:
        void PrePass(IUILayer *layer);
        void StylePass();
        void MeasurePass();
        void ModelPass();
        void ArrangePass();
        bool PostPass();

    protected:
        /// Is this transform dirty?
        bool isDirty = true;

        /// Is a child dirty?
        bool isChildDirty = true;

    protected:
        /// Is this transform visible?
        /// ! Respects isHidden, otherwise based on viewport
        bool isVisible = false;

        /// Is this transform marked as hidden?
        bool isHidden = false;

        /// Does this transform depend on it's children?
        bool isChildDependent = false;

        /// Parent
        Core::WeakPtr<IUITransformComponent> parent;

        /// Horizontal alignment of this ui component
        UIHorizontalAlignment horizontalAlignment = UIHorizontalAlignment::eLeft;

        /// Vertical alignment of this ui component
        UIVerticalAlignment verticalAlignment = UIVerticalAlignment::eTop;

        /// Horizontal scaling of this ui component
        UIScaling horizontalScaling = UIScaling::eNone;

        /// Vertical scaling of this ui component
        UIScaling verticalScaling = UIScaling::eNone;

        /// Content mode of this ui component, defines how this component contributes to the parents size
        UIContentMode contentMode = UIContentMode::eInclude;

        /// Viewport mode of this ui component
        UIViewportMode viewportMode = UIViewportMode::eInherit;

        /// Margin of this ui component
        /* ╔════════════════════════════════
         * ║  Parent    ║
         * ║            ║ <- margin.y
         * ║ ========== ╔═════════════════
         * ║  ^         ║
         * ║   margin.x ║   UI Component
         * ║            ║
         * */
        Math::Vector2 margin;

        /// The desired size of this ui component
        Math::Vector2 desiredSize = {100, 100};
        Math::Vector2 measuredSize = {100, 100};

        /// Bounding box of the desired size
        Math::Vector2 desiredSizeMin = {0, 0};
        Math::Vector2 desiredSizeMax = {100, 100};

        /// Viewport of this component
        Math::Vector4 viewport = {-1e6, -1e6, 1e6, 1e6};

        /// ZWeight
        Int32 zOffset = 0;
        Int32 zAbsolute = 0;

        /// Layer this transform belongs to
        IUILayer *layer = nullptr;

    protected:
        Core::WeakPtr<IUIElement> traceElement = nullptr;

        bool isTraceFallthrough = false;

    protected:
        /// User transforms
        Math::Vector2 userTranslation = {0, 0};
        Math::Quaternion userRotation = Math::Quaternion::Identity();
        Math::Vector2 userScale = {1, 1};

        Math::Vector2 actualTranslation;

        /// Final transform, generated
        Math::Matrix4 pixelTransform = Math::Matrix4::Identity();

        /// Commit version
        Int64 commitVersion = 0;

        /// Dirty children
        Core::Array<Core::WeakPtr<IUITransformComponent>> dirtyChildren;

    private:
        bool wasPreviouslyVisible = false;

        bool cacheResult = true;

        bool isAdded = false;
    };
}