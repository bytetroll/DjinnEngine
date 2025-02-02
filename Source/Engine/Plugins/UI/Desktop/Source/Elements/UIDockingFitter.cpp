#include <UI/Elements/UIDockingFitter.h>
#include <UI/ElementComponents/UILayerTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/ElementComponents/UIInputComponent.h>

DJINN_NS2(UI);

UIDockingFitter::UIDockingFitter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIElement(registry, outer, host) {

}

void UIDockingFitter::StaticInitialize() {
    auto transform = CreateComponent<UILayerTransformComponent>();

    {
        if (!GetSystem()->CreateActor(GetScene(), &mergeImage)) {
            return;
        }

        auto imTransform = mergeImage->GetTransform();
        imTransform->SetAlignment(UIHorizontalAlignment::eCenter, UIVerticalAlignment::eCenter);
        imTransform->SetDesiredSize(imTransform->GetDesiredSize() * .75f);
        transform->AddChild(imTransform);

        //mergeImage->CreateComponent<UIInputComponent>()->BindDragged()
    }

    {
        if (!GetSystem()->CreateActor(GetScene(), &topImage)) {
            return;
        }

        auto imTransform = topImage->GetTransform();
        imTransform->SetAlignment(UIHorizontalAlignment::eCenter, UIVerticalAlignment::eTop);
        imTransform->SetDesiredSize(imTransform->GetDesiredSize() * Math::Vector2(.5f, .1f));
        transform->AddChild(imTransform);
    }

    {
        if (!GetSystem()->CreateActor(GetScene(), &leftImage)) {
            return;
        }

        auto imTransform = leftImage->GetTransform();
        imTransform->SetAlignment(UIHorizontalAlignment::eLeft, UIVerticalAlignment::eCenter);
        imTransform->SetDesiredSize(imTransform->GetDesiredSize() * Math::Vector2(.1f, .5f));
        transform->AddChild(imTransform);
    }

    {
        if (!GetSystem()->CreateActor(GetScene(), &bottomImage)) {
            return;
        }

        auto imTransform = bottomImage->GetTransform();
        imTransform->SetAlignment(UIHorizontalAlignment::eCenter, UIVerticalAlignment::eBottom);
        imTransform->SetDesiredSize(imTransform->GetDesiredSize() * Math::Vector2(.5f, .1f));
        transform->AddChild(imTransform);
    }

    {
        if (!GetSystem()->CreateActor(GetScene(), &rightImage)) {
            return;
        }

        auto imTransform = rightImage->GetTransform();
        imTransform->SetAlignment(UIHorizontalAlignment::eRight, UIVerticalAlignment::eCenter);
        imTransform->SetDesiredSize(imTransform->GetDesiredSize() * Math::Vector2(.1f, .5f));
        transform->AddChild(imTransform);
    }

    SetTransform(transform);
}

void UIDockingFitter::SetDockable(IDockable *dockable) {
    auto slots = dockable->GetDockingSlots();

    mergeImage->GetTransform()->SetHidden(!(slots & EDockingSlot::eMerge));
    topImage->GetTransform()->SetHidden(!(slots & EDockingSlot::eTop));
    leftImage->GetTransform()->SetHidden(!(slots & EDockingSlot::eLeft));
    bottomImage->GetTransform()->SetHidden(!(slots & EDockingSlot::eBottom));
    rightImage->GetTransform()->SetHidden(!(slots & EDockingSlot::eRight));
}
