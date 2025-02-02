//
// Created by Dev on 8/20/2018.
//
#pragma once

#include "IUITransformComponent.h"

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CORE UILeafTransformComponent : public IUITransformComponent {
    public:
        DJINN_COM_CLASS();

        UILeafTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        Math::Vector2 Measure() override;
        void Arrange() override;
        IUITransformComponent *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) override;
        IUITransformComponent *FindChild(const Core::StringView &name) override;

    protected:
        /// Overrides
        bool OnChildDetached(IUITransformComponent *child) override;
    };
}