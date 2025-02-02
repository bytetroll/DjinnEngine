//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UITabItem.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>

DJINN_NS2(UI);

UITabItem::UITabItem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UITabItem::StaticInitialize() {
    auto stack = CreateComponent<UIStackTransformComponent>();
    stack->SetPadding(5.0f);

    SetTransform(stack);
}

void UITabItem::AddItem(IUIElement* element) {

}
