//
// Created by Dev on 3/1/2018.
//

#include <UI/UIWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Game/IEntitySystem.h>
#include <Graphics/IPacketSystem.h>
#include <UI/UIInputSystem.h>
#include <UI/UICanvas.h>

DJINN_NS2(UI);

UIWorldPackage::UIWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result UIWorldPackage::Initialize(Game::IWorld *world) {
    // Register canvas scene
    world->RegisterSceneType<UICanvas>();

    // Install input system
    UIInputSystem *inputSystem;
    if (!registry->CreateClass(world, &inputSystem) || !inputSystem->Initialize() || !world->GetPipeline()->AddInterface(inputSystem) || !world->Register(inputSystem)) {
        ErrorLog(this).Write("Failed to install ui input system into game system");
        return COM::kError;
    }

    // Get packet system
    Graphics::IPacketSystem *packetSystem;
    if (!world->GetPipeline()->GetInterface(&packetSystem)) {
        ErrorLog(this).Write("Camera plugin expected an installed packet system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result UIWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result UIWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
