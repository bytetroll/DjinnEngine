//
// Created by Dev on 3/1/2018.
//

#include <Editor/HierarchyWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IScene.h>
#include <Editor/ILevelLayoutSceneState.h>
#include <Editor/HierarchyHost.h>

DJINN_NS2(Editor);

HierarchyWorldPackage::HierarchyWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result HierarchyWorldPackage::Initialize(Game::IScene *scene) {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(scene->GetState());
    if (!state) {
        return COM::kOK;
    }

    // Create worldHost
    HierarchyHost* host;
    if (!registry->CreateClass(this, &host) || !state->GetPipeline()->AddInterface(host)) {
        ErrorLog(this).Write("Failed to install hierarchy worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result HierarchyWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result HierarchyWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
