//
// Created by Dev on 8/27/2018.
//

#include <Game/SpatialScene.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Game/SceneStructure.h>
#include <Game/ISceneProxy.h>
#include <Game/ISceneProxySystem.h>

DJINN_NS2(Game);

SpatialScene::SpatialScene(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISpatialScene(registry, outer, host) {

}

COM::Result SpatialScene::Initialize(IWorld* world, ISceneState* state) {
    this->world = world;
    this->state = state;

    // Create structure
    SceneStructure *defaultStructure;
    CheckMsg(registry->CreateClass(this, &defaultStructure), Error, "Failed to create default scene structure");

    // ...
    structure = defaultStructure;

    // OK
    return COM::kOK;
}

void SpatialScene::Register(ISceneProxy *proxy) {
    structure->Register(proxy);
}

void SpatialScene::Deregister(ISceneProxy *proxy) {
    structure->Deregister(proxy);
}

void SpatialScene::SetStructure(ISceneStructure *structure) {
    this->structure = structure;
}

ISceneStructure *SpatialScene::GetStructure() {
    return structure;
}

COM::Result SpatialScene::Build() {
    // Build structure
    Check(structure->Build());

    // OK
    return COM::kOK;
}

COM::Result SpatialScene::BuildViewport(COM::IQueryable *queryable, const Math::Matrix4 &transform) {
    auto viewport = PopViewport();

    // Build from structure
    Check(structure->BuildViewport(queryable, viewport, transform));

    // Build all systems from interface list
    for (auto &&interfaceState : viewport->interfaces) {
        auto system = systemMap[interfaceState.iid];

        // Build system from requests
        system->Build(queryable, interfaceState.requests);
    }

    // Release viewport
    {
        Platform::LockGuard guard(viewportPoolMtx);
        viewportPool.Push(viewport);
    }

    // OK
    return COM::kOK;
}

SceneStructureViewport *SpatialScene::PopViewport() {
    // Prepare viewport
    SceneStructureViewport *viewport;
    {
        Platform::LockGuard guard(viewportPoolMtx);
        viewport = viewportPool.Pop();
    }

    // Clear any previous states
    viewport->ClearInterfaces();

    // Remove dead interfaces
    viewport->interfaces.RemoveAll([&](auto &&value) { return !systemMap.Contains(value.iid); });

    // Add new interfaces
    for (auto &&system : systems) {
        if (!viewport->interfaces.Any([&](auto &&value) { return value.iid == system->GetProxyInterfaceID(); })) {
            auto &&view = viewport->interfaces.Emplace();
            view.iid = system->GetProxyInterfaceID();
        }
    }

    // OK
    return viewport;
}

COM::Result SpatialScene::Register(ISceneProxySystem *proxy) {
    systems.Add(proxy);
    systemMap.Add(proxy->GetProxyInterfaceID(), proxy);
    return COM::kOK;
}

COM::Result SpatialScene::Deregister(ISceneProxySystem *proxy) {
    systems.RemoveValue(proxy);
    systemMap.Remove(proxy->GetProxyInterfaceID());
    return COM::kOK;
}

IWorld *SpatialScene::GetWorld() {
    return world;
}

ISceneState *SpatialScene::GetState() {
    return state;
}
