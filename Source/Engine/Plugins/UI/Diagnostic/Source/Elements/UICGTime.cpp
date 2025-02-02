//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UILabel.h>
#include <Game/IActorSystem.h>
#include <Base/DefaultLogs.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <GHA/IInterpreter.h>
#include <Game/IGameSystem.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/IGraphicsSystem.h>

DJINN_NS2(UI);

UICGTime::UICGTime(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UICGTime::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&system), Error, "Expected game system to be installed");

    // Get graphics system
    Graphics::IGraphicsSystem *graphicsSystem;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&graphicsSystem), Error, "Expected graphics system to be installed");

    // Get memory
    interpreter = graphicsSystem->GetInterpreter();

    CheckVoid(GetSystem()->CreateActor(GetScene(), &label));

    SetTransform(label->GetTransform());
}

void UICGTime::Update(double delta) {
    double weight = 0.1;

    avgCDelta = (1.0 - weight) * avgCDelta + weight * system->GetDelta();
    avgGDelta = (1.0 - weight) * avgGDelta + weight * interpreter->GetGPUDelta();

    if ((elapsed += delta) > 0.05) {
        elapsed = 0;
        label->SetText(Core::String("CPU ") + static_cast<int>(1.0 / avgCDelta) + " | GPU " + static_cast<int>(1.0 / avgGDelta));
    }
}
