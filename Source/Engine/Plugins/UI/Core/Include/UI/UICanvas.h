//
// Created by Dev on 8/22/2018.
//

#include <Graphics/IPacketBatcher.h>
#include "IUICanvas.h"

namespace Djinn::Graphics {
    class IOutput;
}

namespace Djinn::UI {
    class CanvasLayout;

    class DJINN_EXPORT_PLUGINS_UI_CORE UICanvas : public IUICanvas, public Graphics::IPacketBatcher {
    public:
        using IUICanvas::kIID;

        DJINN_COM_CLASS();

        UICanvas(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result Initialize(Game::IWorld *world, Game::ISceneState *state) override;
        COM::Result Build() override;
        void Register(IUIRenderComponent *component) override;
        void Deregister(IUIRenderComponent *component) override;
        bool Trace(const Math::Vector2 &position, Math::Vector2 *out) override;
        IUIElement *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) override;
        void AddElement(IUIElement *element) override;
        void RemoveElement(IUIElement *element) override;
        COM::Result Build(Graphics::WorldPacket *batch) override;
        COM::Result Build(Graphics::UIPacket* packet, UILayerPass pass) override;
        void Register(IUILayer *layer) override;
        void Deregister(IUILayer *layer) override;
        Game::IWorld *GetWorld() override;
        Game::ISceneState *GetState() override;
        void SetProxy(IUICanvas* canvas) override;
        Style *GetStyle() override;

    private:
        Game::IWorld* world = nullptr;
        Game::ISceneState* state = nullptr;

        Style* style = nullptr;

        Core::Array<Core::WeakPtr<IUIRenderComponent>> renderComponents;
        Core::Array<IUILayer*> layers;

        IUICanvas* proxy = nullptr;
        CanvasLayout* canvasLayout = nullptr;
    };
}