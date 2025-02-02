//
// Created by Dev on 8/22/2018.
//

#include <Game/ActorCallback.h>
#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CORE TransformDebug : public IUIElement {
    public:
        DJINN_COM_CLASS();

        TransformDebug(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        void StaticInitialize() override;

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;
        };
    };
}