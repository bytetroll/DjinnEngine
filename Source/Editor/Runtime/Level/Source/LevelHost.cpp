//
// Created by Dev on 7/11/2018.
//

#include <Editor/LevelHost.h>
#include <Editor/Level.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>
#include "../../Layout/Include/Editor/WorldFeatures.h"

DJINN_NS2(Editor);

LevelHost::LevelHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILevelHost(registry, outer, host) {

}

COM::Result LevelHost::Initialize() {
    // Get world worldHost
    Game::IWorldHost *worldHost;
    if (!registry->GetPipeline()->GetInterface(&worldHost)) {
        ErrorLog(this).Write("Expected world worldHost to be installed");
        return COM::kOK;
    }

    // Create editor feature
    Game::IWorldFeature* feature;
    if (!worldHost->CreateFeature(kEditorFeature, &feature)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result LevelHost::CreateLevel(ILevel **out) {
    Level* level;
    if (!registry->CreateClass(this, &level) || !level->Initialize()) {
        ErrorLog(this).Write("Failed to create level");
        return COM::kError;
    }

    // OK
    *out = level;
    return COM::kOK;
}

COM::Result LevelHost::SaveLevel(ILevelAsset **out) {
    return COM::kNotImplemented;
}

COM::Result LevelHost::LoadLevel(ILevelAsset *asset) {
    return COM::kNotImplemented;
}
