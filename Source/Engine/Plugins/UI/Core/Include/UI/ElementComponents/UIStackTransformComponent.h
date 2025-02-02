//
// Created by Dev on 8/20/2018.
//
#pragma once

#include "IUITransformComponent.h"
#include <Core/Annotation.h>
#include <Reflection/Attributes/Proxy.h>

namespace Djinn::UI {
    ClassAttr(
            EnumProxy("eHorizontal", "horizontal"),
            EnumProxy("eVertical", "vertical"),
            EnumProxy("eScanGrid", "scangrid")
    )
    enum class UIStackMode {
        eHorizontal,
        eVertical,
        eScanGrid
    };

    class DJINN_EXPORT_PLUGINS_UI_CORE UIStackTransformComponent : public IUITransformComponent {
    public:
        DJINN_COM_CLASS();

        UIStackTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Add a child to this layout
        /// \param child
        void AddChild(IUITransformComponent *child);

        /// Remove a child from this layout
        /// \param child
        void RemoveChild(IUITransformComponent *child);

        /// Overrides
        Math::Vector2 Measure() override;
        void Arrange() override;
        IUITransformComponent *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) override;
        IUITransformComponent *FindChild(const Core::StringView &name) override;

    protected:
        /// Overrides
        bool OnChildDetached(IUITransformComponent *child) override;
        void OnDirty() override;

    public:
        /// Set padding
        /// \param padding
        void SetPadding(float padding) {
            this->padding = padding;
            MarkAsDirty();
        }

        /// Get padding
        /// \return
        float GetPadding() const {
            return padding;
        }

        /// Set padding
        /// \param padding
        void SetMode(UIStackMode mode) {
            this->mode = mode;
            MarkAsDirty();
        }

        /// Get padding
        /// \return
        UIStackMode GetMode() const {
            return mode;
        }

    protected:
        /// Element padding
        float padding = 3.0f;

        /// Stack desired size
        Math::Vector2 stackDesiredSize;

        /// Count of filled children
        Int32 fillCount = 0;

        /// Stack mode
        UIStackMode mode = UIStackMode::eHorizontal;

        /// All children
        Core::Array<Core::WeakPtr<IUITransformComponent>> children;
    };
}