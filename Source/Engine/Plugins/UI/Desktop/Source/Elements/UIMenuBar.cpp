//
// Created by Dev on 8/31/2018.
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
