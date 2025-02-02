//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>

namespace Djinn::Graphics {
    class UIDrawPacketBuffer;
}

namespace Djinn::UI {
    /// The rendering policy of a ui element
    /// A render is always invoked if an area is invalidated regardless of the rendering policy
    enum class UIRenderPolicy {
        /// Always render the element
                eAlways,

        /// Only rendered when the transform has changed
                eTransformChanged,
    };

    class DJINN_EXPORT_PLUGINS_UI_CORE IUIRenderComponent : public IUIElementComponent {
    public:
        DJINN_COM_INTERFACE();

        IUIRenderComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

        }

        /// Build this render component
        /// \param batch
        virtual void Build(Graphics::UIDrawPacketBuffer *buffer) = 0;

        /// Overrides
        void Showed(IUILayer *layer) override;
        void Hidden(IUILayer *layer) override;
        void Destroyed() override;

    public:
        /// Set the rendering policy
        /// \param policy
        void SetRenderPolicy(UIRenderPolicy policy);

        /// Get the rendering policy
        /// \return
        UIRenderPolicy GetRenderPolicy() const {
            return policy;
        }

        /// Mark this render component as invalidated
        void MarkAsInvalidated() {
            isInvalidated = true;
        }

        /// Is this component invalidated?
        /// \return
        bool IsInvalidated() const {
            return isInvalidated;
        }

    protected:
        /// Is this renderer invalidated?
        bool isInvalidated = false;

        /// The render policy
        UIRenderPolicy policy = UIRenderPolicy::eTransformChanged;

    private:
        IUILayer* registeredLayer = nullptr;
    };
}