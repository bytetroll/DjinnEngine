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

#include <UI/Elements/UICheckbox.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UILabel.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UIInputComponent.h>

DJINN_NS2(UI);

UICheckbox::UICheckbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UICheckbox::StaticInitialize() {
    auto layout = CreateComponent<UILayoutTransformComponent>();
    layout->SetScaling(UIScaling::eContent, UIScaling::eContent);

    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    onTexture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/CheckboxOnMB.png"_path);
    offTexture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/CheckboxOffMB.png"_path);

    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &img));

        img->SetTexture(offTexture);

        auto input = img->CreateComponent<UIInputComponent>();
        input->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *buffer) {
            SetChecked(!isChecked);
        });

        auto transform = img->GetTransform();
        transform->SetDesiredSize({15, 15});
        layout->AddChild(transform);
    }

    SetTransform(layout);
}

void UICheckbox::SetChecked(bool value) {
    isChecked = value;
    img->SetTexture(isChecked ? onTexture : offTexture);
    checked.Signal(this, checked);
}
