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

#include <UI/Elements/UIButton.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UILabel.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UIInputComponent.h>

DJINN_NS2(UI);

UIButton::UIButton(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {
}

void UIButton::StaticInitialize() {
    auto layout = CreateComponent<UILayoutTransformComponent>();
    layout->SetDesiredSize({100, 20});

    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &img));
        img->SetTexture(host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/ButtonMB.png"_path));

        img->SetMarginGeometry({3, 3});

        auto transform = img->GetTransform();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        transform->SetTraceFallthrough(true);
        transform->SetViewportMode(UIViewportMode::eKeep);
        //transform->SetViewportMode(UIViewportMode::eNone);
    }
    layout->AddChild(img->GetTransform());

    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &lbl));
        lbl->SetText("Click Me");

        auto transform = lbl->GetTransform();
        transform->SetAlignment(UIHorizontalAlignment::eCenter, UIVerticalAlignment::eCenter);
        transform->SetDepthOffset(1);
        transform->SetTraceFallthrough(true);
        transform->SetViewportMode(UIViewportMode::eKeep);
    }
    layout->AddChild(lbl->GetTransform());

    auto input = CreateComponent<UIInputComponent>();
    {
        input->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed& event, UIInputActionBuffer* buffer) {
            pressed.Signal(this);

            img->SetColor({1.2f, 1.2f, 1.2f, 1.0f});
            GetTransform()->SetUserScale({0.95f, 0.95f});
        });

        input->mouseReleased.Add(this, [=](const Game::InputEvent::MouseReleased& event, IUIElement* element, UIInputActionBuffer* buffer) {
            img->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
            GetTransform()->SetUserScale({1.0f, 1.0f});
        });
    }

    SetTransform(layout);
}

void UIButton::SetText(const Core::StringView &text) {
    lbl->SetText(text.ToString());
}

Core::StringView UIButton::GetText() const {
    return lbl->GetText();
}

void UIButton::LoadTexture(const Core::StringView &path) {
    img->LoadTexture(path);
}

void UIButton::SetTexture(const Data::Asset<Data::ITextureAsset> &texture) {
    img->SetTexture(texture);
}

const Data::Asset<Data::ITextureAsset> &UIButton::GetTexture() const {
    return img->GetTexture();
}

void UIButton::SetBoxGeometry() {
    img->SetBoxGeometry();
}

void UIButton::SetMarginGeometry(const Math::Vector2 &margin) {
    img->SetMarginGeometry(margin);
}
