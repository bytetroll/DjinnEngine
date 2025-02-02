//
// Created by Dev on 8/23/2018.
//

#include <UI/Elements/UIFpsLabel.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <Game/IGameSystem.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UILabel.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

UIFpsLabel::UIFpsLabel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIFpsLabel::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&gameSystem), Error, "Expected game system to be installed");

    CheckVoid(GetSystem()->CreateActor(GetScene(), &label));

    SetTransform(label->GetTransform());
}

void UIFpsLabel::Update(double delta) {
    double weight = 0.1;
    avgDelta = (1.0 - weight) * avgDelta + weight * gameSystem->GetDelta();

    if ((elapsed += delta) > 0.05) {
        elapsed = 0;
        label->SetText(Core::String("FPS (") + gameSystem->GetMaxIPS() + " Cap): " + static_cast<int>(1.0 / avgDelta));
    }
}

