//
// Created by Dev on 8/23/2018.
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
