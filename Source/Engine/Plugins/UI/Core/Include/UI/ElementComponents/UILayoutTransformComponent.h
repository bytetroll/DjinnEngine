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