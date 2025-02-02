//
// Created by Dev on 3/1/2018.
//

#include <Editor/ViewportWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IScene.h>
#include <Editor/ILevelLayoutSceneState.h>
#include <Editor/ViewportHost.h>

DJINN_NS2(Editor);

ViewportWorldPackage::ViewportWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result ViewportWorldPackage::Initialize(Game::IScene *scene) {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(scene->GetState());
    if (!state) {
        return COM::kOK;
    }

    // Create worldHost
    ViewportHost* host;
    if (!registry->CreateClass(this, &host) || !state->GetPipeline()->AddInterface(host)) {
        ErrorLog(this).Write("Failed to install viewport worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result ViewportWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result ViewportWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
