//
// Created by Dev on 9/1/2018.
//

#include <UI/IControl.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <UI/IUIMLHost.h>
#include <UI/IUICanvas.h>

DJINN_NS2(UI);

IControl::IControl(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void IControl::Construct(const Base::PathID& path) {
    this->path = path;

    auto layout = CreateComponent<UILayoutTransformComponent>();

    // Default transform
    SetTransform(layout);

    // Get asset worldHost
    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get uiml worldHost
    IUIMLHost* uiml;
    CheckMsgVoid(world->GetPipeline()->GetInterface(&uiml), Error, "Expected UIML worldHost to be installed");

    // Build from path
    CheckVoid(uiml->Build(COM::CastChecked<IUICanvas>(GetScene()), host->CompileAssetAsync<Data::IUIMLAsset>(path).Get(), this, [&](IUIElement* element) {
        layout->AddChild(element->GetTransform());
    }));
}
