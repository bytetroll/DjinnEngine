//
// Created by Dev on 8/20/2018.
//
#pragma once

#include "IUITransformComponent.h"

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CORE UILayoutTransformComponent : public IUITransformComponent {
    public:
        DJINN_COM_CLASS();

        UILayoutTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

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
        void StyleEvent(const ElementStyle &style) override;

    protected:
        /// Overrides
        bool OnChildDetached(IUITransformComponent *child) override;
        void OnDirty() override;

    public:
        /// Set padding
        /// \param padding
        void SetPadding(const Math::Vector4 &padding) {
            this->padding = padding;
            MarkAsDirty();
        }

        /// Get padding
        /// \return
        const Math::Vector4& GetPadding() const {
            return padding;
        }

        /// Get all children
        /// \return
        Core::ArrayView<Core::WeakPtr<IUITransformComponent>> GetChildren() {
            return children;
        }

    protected:
        /// Padding of this ui component (Only applies to children)
        /* ╔═══════════════════════════════════════════════════════╗
         * ║  Com       ║                                  ║
         * ║            ║ <- padding.y                     ║
         * ║ ========== ╔══════════════════════════╗           ║
         * ║  ^         ║                      ║           ║
         * ║  padding.x ║   Child Component    ║           ║
         * ║            ║                      ║           ║
         * ║            ╚══════════════════════════╝ ========= ║
         * ║                      padding.w -> ║  ^        ║
         * ║                                   ║  padding.z║
         * ╚═══════════════════════════════════════════════════════╝
         * */
        Math::Vector4 padding;

        /// All children
        Core::Array<Core::WeakPtr<IUITransformComponent>> children;
    };
}