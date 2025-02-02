//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UITreeView.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>
#include <UI/Elements/UIStack.h>

DJINN_NS2(UI);

UITreeView::UITreeView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

}

void UITreeView::StaticInitialize() {
    Construct("Runtime/Engine/UIML/TreeView.uiml"_path);

    stack = FindChild<UI::UIStack>("items");
}

void UITreeView::AddChild(IUITransformComponent *child) {
    stack->AddChild(child);
}

void UITreeView::RemoveChild(IUITransformComponent *child) {
    stack->RemoveChild(child);
}
