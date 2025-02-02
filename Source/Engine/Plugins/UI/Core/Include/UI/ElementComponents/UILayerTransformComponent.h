//
// Created by Dev on 8/20/2018.
//
#pragma once

#include "IUITransformComponent.h"
#include <UI/IUILayer.h>

namespace Djinn::UI {
    struct UILayerTransformInfo {
        IUILayer* layer{nullptr};
        IUITransformComponent* transform{nullptr};
        USize priority = 0;
    };

    class DJINN_EXPORT_PLUGINS_UI_CORE UILayerTransformComponent : public IUITransformComponent, public IUILayer {
    public:
        DJINN_COM_CLASS();

        UILayerTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Add a child
        /// \param transform
        /// \param layer
        void AddChild(IUITransformComponent* transform, IUILayer* layer = nullptr);

        /// Remove a child
        /// \param transform
        void RemoveChild(IUITransformComponent* transform);

        /// Set the priority of a transform
        /// \param transform
        /// \param priority
        void SetPriority(IUITransformComponent* transform, USize priority);

        /// Get the priority of a transform
        /// \param transform
        /// \return
        USize GetPriority(IUITransformComponent* transform);

        /// Overrides
        Math::Vector2 Measure() override;
        void Arrange() override;
        IUITransformComponent *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) override;
        IUITransformComponent *FindChild(const Core::StringView &name) override;
        void Register(IUILayer *layer) override;
        void Deregister(IUILayer *layer) override;
        void Register(IUIRenderComponent *component) override;
        void Deregister(IUIRenderComponent *component) override;
        COM::Result Build(Graphics::UIPacket *packet, UILayerPass pass) override;
        IUILayer *RequestLayer(IUILayer *layer) override;
        void OnDirty() override;

    protected:
        /// Overrides
        bool OnChildDetached(IUITransformComponent *child) override;

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

        /// Get all layers
        /// \return
        Core::ArrayView<UILayerTransformInfo> GetLayers() {
            return layers;
        }

    protected:
        void SortLayers();

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

        Core::Array<Core::WeakPtr<IUIRenderComponent>> renderComponents;

        Core::Array<UILayerTransformInfo> layers;
        Core::Array<IUILayer*> loseLayers;

        IUILayer* parentLayer = nullptr;
    };
}