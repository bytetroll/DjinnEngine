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
 on 9/1/2018.
//

#include <UI/IControl.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <UI/IUIMLHost.h>
#include <UI/IUICanvas.h>

DJINN_NS2(UI);

IControl::IControl(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void IControl::Construct(const Base::PathID& path) {
    this->path = path;

    auto layout = CreateComponent<UILayoutTransformComponent>();

    // Default transform
    SetTransform(layout);

    // Get asset worldHost
    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get uiml worldHost
    IUIMLHost* uiml;
    CheckMsgVoid(world->GetPipeline()->GetInterface(&uiml), Error, "Expected UIML worldHost to be installed");

    // Build from path
    CheckVoid(uiml->Build(COM::CastChecked<IUICanvas>(GetScene()), host->CompileAssetAsync<Data::IUIMLAsset>(path).Get(), this, [&](IUIElement* element) {
        layout->AddChild(element->GetTransform());
    }));
}
