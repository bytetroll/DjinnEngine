//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/27/2018.
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
