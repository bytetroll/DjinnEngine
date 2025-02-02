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
 on 8/22/2018.
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