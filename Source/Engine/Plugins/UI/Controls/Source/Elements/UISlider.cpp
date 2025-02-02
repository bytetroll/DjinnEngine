//
// Created by Dev on 8/23/2018.
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
