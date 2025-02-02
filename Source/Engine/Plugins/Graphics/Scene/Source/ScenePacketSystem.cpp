//
// Created by Dev on 8/30/2018.
//

#include <Graphics/ScenePacketSystem.h>
#include <Graphics/IScenePacketBatcher.h>
#include <Game/IWorld.h>
#include <Graphics/WorldPacket.h>
#include <Graphics/ScenePacket.h>

DJINN_NS2(Graphics);

ScenePacketSystem::ScenePacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IScenePacketSystem(registry, outer, host) {

}

COM::Result ScenePacketSystem::Initialize(Game::IWorld *world) {
    this->world = world;

    // OK
    return COM::kOK;
}

COM::Result ScenePacketSystem::Build(WorldPacket* batch) {
    // Get buffer
    auto sceneBuffer = batch->batch.GetPacket<ScenePacketBuffer>();

    // Process all scenes
    for (auto scene : world->GetScenes()) {
        // Create scene packet
        auto scenePacket = sceneBuffer->Add();

        // Run all batchers
        Async::Group group;
        for (auto batcher : batchers) {
            batcher->BuildAsync(group, scene, scenePacket);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result ScenePacketSystem::Register(IScenePacketBatcher *batcher) {
    batchers.Add(batcher);
    return COM::kOK;
}

COM::Result ScenePacketSystem::Deregister(IScenePacketBatcher *batcher) {
    batchers.RemoveValue(batcher);
    return COM::kOK;
}
