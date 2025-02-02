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
 on 8/23/2018.
//

#include <UI/Elements/UIListbox.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/Elements/UIScrollArea.h>

DJINN_NS2(UI);

UIListbox::UIListbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIListbox::StaticInitialize() {
    auto layout = CreateComponent<UIStackTransformComponent>();

    {
        UIScrollArea* scrollArea;
        CheckVoid(GetSystem()->CreateActor(GetScene(), &scrollArea));

        auto stack = scrollArea->CreateComponent<UIStackTransformComponent>();

        items = stack;

        scrollArea->SetContent(stack);

        layout->AddChild(scrollArea->GetTransform());
    }

    SetTransform(layout);
}

void UIListbox::AddItem(IUITransformComponent *transform) {
    items->AddChild(transform);
}

