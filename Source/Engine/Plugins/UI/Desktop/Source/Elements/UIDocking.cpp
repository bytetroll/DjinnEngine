//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UIDocking.h>
#include <UI/Elements/IDockable.h>
#include <UI/Elements/UIDockingManager.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/ElementComponents/UILayerTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>
#include <UI/Elements/UIStack.h>
#include <UI/Elements/UIWindow.h>

DJINN_NS2(UI);

UIDocking::UIDocking(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIDocking::StaticInitialize() {
    Data::IAssetHost *host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get texture assets
    auto textureBackground = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/MenubarMB.png"_path);

    auto layout = CreateComponent<UILayoutTransformComponent>();
    layout->SetScaling(UIScaling::eNone, UIScaling::eNone);

    UIImage *img;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &img));

        img->SetTexture(textureBackground);
    }
    {
        auto transform = img->GetTransform();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        layout->AddChild(transform);
    }

    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &stack));
        stack->SetName("tits");
        stack->SetResizable(true);

        auto transform = stack->GetTransform();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        layout->AddChild(transform);
    }

    SetTransform(layout);
}

void UIDocking::SetScale(bool scale) {
    auto scaling = scale ? UIScaling::eFill : UIScaling::eNone;
    GetTransform()->SetScaling(scaling, scaling);
}

void UIDocking::SetMerged(bool scale) {

}

void UIDocking::AddChild(IUITransformComponent *child) {
    IDockable* dockable;
    if (!child->GetElement()->IQueryable::QueryInterface(&dockable)) {
        ErrorLog(this).Write("Attempting to add child '", child->GetElement()->GetName(), "' without an IDockable interface");
        return;
    }

    dockable->SetDockingReference(this);
    dockable->SetDockingManagerReference(GetDockingManager());

    dockables.Add(dockable);
    stack->AddChild(child);

    switch (mode) {
        case UIDockingMode::eVertical:
            child->SetHorizontalScaling(UIScaling::eFill);
            break;
        case UIDockingMode::eHorizontal:
            child->SetVerticalScaling(UIScaling::eFill);
            break;
    }
}

void UIDocking::RemoveChild(IUITransformComponent *child) {
    IDockable* dockable;
    if (!child->GetElement()->IQueryable::QueryInterface(&dockable)) {
        ErrorLog(this).Write("Attempting to remove child '", child->GetElement()->GetName(), "' without an IDockable interface");
        return;
    }

    dockable->SetDockingReference(nullptr);
    dockable->SetDockingManagerReference(nullptr);

    dockables.RemoveValue(dockable);
    stack->RemoveChild(child);

    // Empty dockers are not valid
    if (stack->IsEmpty()) {
        // Parent docker?
        if (auto docking = GetDocking()) {
            docking->RemoveChild(GetTransform());
        }

        Destroy();
    }
}

void UIDocking::SetMode(UIDockingMode mode) {
    this->mode = mode;
    switch (mode) {
        case UIDockingMode::eVertical:
            stack->SetMode(UIStackMode::eVertical);
            break;
        case UIDockingMode::eHorizontal:
            stack->SetMode(UIStackMode::eHorizontal);
            break;
    }
}

IUILayer *UIDocking::GetLayer() {
    return static_cast<UILayerTransformComponent*>(GetTransform())->GetLayer();
}

Core::ArrayView<IDockable *> UIDocking::GetDockables() {
    return dockables;
}

IUIElement *UIDocking::GetDockingElement() {
    return this;
}

EDockingSlotSet UIDocking::GetDockingSlots() {
    switch (mode) {
        case UIDockingMode::eHorizontal:
            return EDockingSlot::eLeft | EDockingSlot::eMerge | EDockingSlot::eRight;
        case UIDockingMode::eVertical:
            return EDockingSlot::eTop | EDockingSlot::eMerge | EDockingSlot::eBottom;
    }

    return EDockingSlot::eNone;
}

void UIDocking::AddDockable(EDockingSlot slot, IDockable *dockable) {

}
