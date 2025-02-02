//
// Created by Dev on 3/1/2018.
//

#include <Editor/TemplatesWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IScene.h>
#include <Editor/ILevelLayoutSceneState.h>
#include <Editor/TemplatesHost.h>

DJINN_NS2(Editor);

TemplatesWorldPackage::TemplatesWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result TemplatesWorldPackage::Initialize(Game::IScene *scene) {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(scene->GetState());
    if (!state) {
        return COM::kOK;
    }

    // Create worldHost
    TemplatesHost* host;
    if (!registry->CreateClass(this, &host) || !state->GetPipeline()->AddInterface(host)) {
        ErrorLog(this).Write("Failed to install templates worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result TemplatesWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result TemplatesWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
