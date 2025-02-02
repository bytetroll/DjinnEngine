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
 on 8/31/2018.
//

#include <UI/Elements/UIMenuBar.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>

DJINN_NS2(UI);

UIMenuBar::UIMenuBar(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIMenuBar::StaticInitialize() {
    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get texture assets
    auto textureBackground = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/MenubarMB.png"_path);

    auto stack = CreateComponent<UIStackTransformComponent>();
    stack->SetPadding(5.0f);

    UIImage* img;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &img));
        img->GetTransform()->SetScaling(UIScaling::eFill, UIScaling::eFill);
        img->SetTexture(textureBackground);
    }
    stack->AddChild(img->GetTransform());

    SetTransform(stack);
}

void UIMenuBar::AddItem(IUIElement* element) {
    static_cast<UIStackTransformComponent*>(GetTransform())->AddChild(element->GetTransform());
}
