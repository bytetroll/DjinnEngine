//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UICGMemory.h>
#include <UI/Elements/UILabel.h>
#include <Game/IActorSystem.h>
#include <Game/IGameSystem.h>
#include <Base/DefaultLogs.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/IGraphicsSystem.h>

DJINN_NS2(UI);

UICGMemory::UICGMemory(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UICGMemory::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&system), Error, "Expected game system to be installed");

    // Get graphics system
    Graphics::IGraphicsSystem *graphicsSystem;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&graphicsSystem), Error, "Expected graphics system to be installed");

    // Get memory
    memory = graphicsSystem->GetDevice()->GetMemory();

    CheckVoid(GetSystem()->CreateActor(GetScene(), &label));

    SetTransform(label->GetTransform());
}

void UICGMemory::Update(double delta) {
    if ((elapsed += delta) > 0.05) {
        elapsed = 0;
        label->SetText(Core::String("GPU Memory: ") + (memory->GetAllocatedMemory() / 1'000'000) + "mb [" + (memory->GetUsedMemory() / 1'000'000) + "mb used]");
    }
}
