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

#include <UI/Elements/UISlider.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIImage.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/ElementComponents/UIDragDropComponent.h>

DJINN_NS2(UI);

UISlider::UISlider(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UISlider::StaticInitialize() {
    auto layout = CreateComponent<UILayoutTransformComponent>();
    layout->SetDesiredSize({80, 12});

    Data::IAssetHost *host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    UIImage *background;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &background));

        background->SetColor({0.55f, 0.55f, 0.55f, 1.0f});
        background->SetTexture(nullptr);

        auto backgroundTransform = background->GetTransform();
        backgroundTransform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
        backgroundTransform->SetHorizontalAlignment(UIHorizontalAlignment::eLeft);
        backgroundTransform->SetHorizontalScaling(UIScaling::eFill);
        backgroundTransform->SetDesiredSize({0, 7});
    }
    layout->AddChild(background->GetTransform());

    UIImage *progress;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &progress));

        progress->SetColor({0.4f, 0.9f, 0.4f, 1.0f});
        progress->SetTexture(nullptr);

        auto backgroundTransform = progress->GetTransform();
        backgroundTransform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
        backgroundTransform->SetHorizontalAlignment(UIHorizontalAlignment::eLeft);
        backgroundTransform->SetDesiredSize({30, 7});
        backgroundTransform->SetDepthOffset(1);
    }
    layout->AddChild(progress->GetTransform());

    UIImage *button;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &button));

        button->SetColor({0.4f, 0.9f, 0.4f, 1.0f});
        button->SetTexture(nullptr);

        auto backgroundTransform = button->GetTransform();
        backgroundTransform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
        backgroundTransform->SetHorizontalAlignment(UIHorizontalAlignment::eLeft);
        backgroundTransform->SetDesiredSize({7, 12});
        backgroundTransform->SetMargin({30, 0});
        backgroundTransform->SetDepthOffset(2);

        auto dd = button->CreateComponent<UIDragDropComponent>();
        dd->SetMask({1, 0});
        dd->SetRestricted(true);

        dd->movedEvents.Add(this, [=] {
            progress->GetTransform()->SetDesiredSize({backgroundTransform->GetMargin().x, progress->GetTransform()->GetDesiredSize().y});
        });
    }
    layout->AddChild(button->GetTransform());

    SetTransform(layout);
}
