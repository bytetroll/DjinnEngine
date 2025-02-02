//
// Created by Dev on 8/23/2018.
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

