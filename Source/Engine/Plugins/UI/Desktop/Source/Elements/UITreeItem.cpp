//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UITreeItem.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>
#include <UI/Elements/UIStack.h>
#include <UI/Elements/UILabel.h>

DJINN_NS2(UI);

UITreeItem::UITreeItem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

}

void UITreeItem::StaticInitialize() {
    Construct("Runtime/Engine/UIML/TreeItem.uiml"_path);

    stack = FindChild<UIStack>("items");
    label = FindChild<UILabel>("text");
}

void UITreeItem::AddChild(IUITransformComponent *child) {
    stack->AddChild(child);
}

void UITreeItem::RemoveChild(IUITransformComponent *child) {
    stack->RemoveChild(child);
}

void UITreeItem::SetText(const Core::StringView &text) {
    label->SetText(text);
}

void UITreeItem::OnExpanded() {
    expanded.Signal(this);
}

void UITreeItem::OnClicked() {
    clicked.Signal(this);
}
