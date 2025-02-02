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
 on 8/31/2018.
//

#include <UI/Elements/UIDockingManager.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/ElementComponents/UILayerTransformComponent.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>
#include <UI/Elements/UIStack.h>
#include <UI/Elements/UIWindow.h>
#include <UI/Elements/UIDocking.h>
#include <UI/Elements/UIDockingFitter.h>
#include <Graphics/UIPacket.h>
#include <Graphics/UILayerPacket.h>
#include <Graphics/UIDrawPacket.h>

DJINN_NS2(UI);

enum class EUIDockingPriority
{
    eStatic,
    eBackground,
    eForeground,
    eFitter
};

UIDockingManager::UIDockingManager(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIDockingManager::StaticInitialize() {
    layout = CreateComponent<UILayerTransformComponent>();
    layout->SetScaling(UIScaling::eNone, UIScaling::eNone);

    auto input = CreateComponent<UIInputComponent>();

    input->propagated.Add(this, [=](const Game::InputEvent &event, IUIElement* element, UIInputActionBuffer *actions) {
        switch (event.type) {
            default:
                break;
            case Game::InputEventType::eMouseMoved:
                mousePosition = event.mouseMoved.position;
                break;
        }
    });

    SetTransform(layout);
}

void UIDockingManager::AddChild(IUITransformComponent *child) {
    IDockable* dockable;
    if (!child->GetElement()->IQueryable::QueryInterface(&dockable)) {
        ErrorLog(this).Write("Attempting to add child '", child->GetElement()->GetName(), "' without an IDockable interface");
        return;
    }

    dockable->SetDockingManagerReference(this);

    layout->AddChild(child, dockable->GetLayer());
}

void UIDockingManager::RemoveChild(IUITransformComponent *child) {
    IDockable* dockable;
    if (!child->GetElement()->IQueryable::QueryInterface(&dockable)) {
        ErrorLog(this).Write("Attempting to remove child '", child->GetElement()->GetName(), "' without an IDockable interface");
        return;
    }

    dockable->SetDockingManagerReference(nullptr);

    layout->RemoveChild(child);
}

void UIDockingManager::SetFloating(IUITransformComponent *transform) {
    layout->SetPriority(transform, static_cast<UInt32>(EUIDockingPriority::eBackground));
}

void UIDockingManager::SetForeground(IUITransformComponent *transform) {
    if (foregroundTransform) {
        layout->SetPriority(foregroundTransform, static_cast<UInt32>(EUIDockingPriority::eBackground));
    }
    layout->SetPriority(transform, static_cast<UInt32>(EUIDockingPriority::eForeground));
    foregroundTransform = transform;
}

bool UIDockingManager::IsFloating(IUITransformComponent *transform) {
    return layout->GetPriority(transform) != static_cast<UInt32>(EUIDockingPriority::eStatic);
}

void UIDockingManager::ShowFitter(IDockable* ignore, IUITransformComponent *transform) {
    // First, ensure we can find a relevant docking element
    auto docking = FindDocking(ignore, mousePosition);
    if (!docking) {
        HideFitter();
        return;
    }

    // Ensure the fitter exists
    if (!fitter) {
        if (!GetSystem()->CreateActor(GetScene(), &fitter)) {
            return;
        }
        
        auto transform = fitter->GetTransform();

        layout->AddChild(transform, COM::Cast<UILayerTransformComponent>(fitter->GetTransform()));
        
        layout->SetPriority(transform, static_cast<UInt32>(EUIDockingPriority::eFitter));
    }

    fitter->SetDockable(docking);

    auto dockingTransform = docking->GetDockingElement()->GetTransform();

    fitter->GetTransform()->SetMargin(dockingTransform->GetAbsolutePosition() - GetTransform()->GetAbsolutePosition() + dockingTransform->GetDesiredSize() * .5f);
}

void UIDockingManager::HideFitter() {
    if (fitter) {
        fitter->Destroy();
    }
}

IDockable *UIDockingManager::FindDocking(IDockable* ignore, const Math::Vector2 &position) {
    // Search non static layers
    for (auto&& layer : layout->GetLayers()) {
        if (layer.priority == static_cast<UInt32>(EUIDockingPriority::eStatic))
            continue;

        IDockable* dockable;
        if (!layer.transform->GetElement()->IQueryable::QueryInterface(&dockable))
            continue;

        if (auto docking = FindDocking(dockable, ignore, position)) {
            return docking;
        }
    }

    // Search static layers
    for (auto&& layer : layout->GetLayers()) {
        if (layer.priority != static_cast<UInt32>(EUIDockingPriority::eStatic))
            continue;

        IDockable* dockable;
        if (!layer.transform->GetElement()->IQueryable::QueryInterface(&dockable))
            continue;

        if (auto docking = FindDocking(dockable, ignore, position)) {
            return docking;
        }
    }

    return nullptr;
}

IDockable *UIDockingManager::FindDocking(IDockable* dockable, IDockable* ignore, const Math::Vector2 &position) {
    if (dockable == ignore)
        return nullptr;

    // Attempt to find child dockers
    for (auto&& childDockable : dockable->GetDockables()) {
        if (auto docking = FindDocking(childDockable, ignore, position)) {
            return docking;
        }
    }

    // Try the root docker
    if (dockable->GetDockingElement()->GetTransform()->IsPointInside(position)) {
        return dockable;
    }

    return nullptr;
}

