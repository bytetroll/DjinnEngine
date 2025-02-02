//
// Created by Dev on 1/21/2019.
//

#include <Editor/LevelLayoutSceneState.h>
#include <Editor/IEditorWorld.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Editor/ILevel.h>
#include <Host/Pipeline.h>

DJINN_NS2(Editor);

LevelLayoutSceneState::LevelLayoutSceneState(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ILevelLayoutSceneState(registry, outer, host) {

}

COM::Result LevelLayoutSceneState::Initialize(ILevel *level) {
    this->level = level;

    // Create pipeline
    pipeline = new Host::Pipeline();

    // OK
    return COM::kOK;
}

ILevel *LevelLayoutSceneState::GetLevel() {
    return level;
}

Host::IPipeline *LevelLayoutSceneState::GetPipeline() {
    return pipeline;
}
