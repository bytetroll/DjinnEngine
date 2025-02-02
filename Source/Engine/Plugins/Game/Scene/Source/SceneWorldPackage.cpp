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
 on 3/1/2018.
//

#include <Game/SceneWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Game/ISpatialScene.h>
#include <Game/IEntitySystem.h>
#include <Game/ICameraHost.h>
#include <Game/CameraHost.h>
#include <Graphics/IPacketSystem.h>
#include <Game/SceneCameraPacketBatcher.h>
#include <Graphics/IScenePacketSystem.h>
#include <Game/Proxies/ISceneMeshProxy.h>
#include <Game/Proxies/ISceneDirectionalLightProxy.h>
#include <Game/SpatialScene.h>

DJINN_NS2(Game);

SceneWorldPackage::SceneWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result SceneWorldPackage::Initialize(IWorld *world) {
    // Register spatial scene
    world->RegisterSceneType<SpatialScene>();

    // Get entity system
    IEntitySystem *entitySystem;
    if (!world->GetPipeline()->GetInterface(&entitySystem)) {
        ErrorLog(this).Write("SpatialScene plugin expected an installed entity system");
        return COM::kError;
    }

    // Get scene packet system
    Graphics::IScenePacketSystem *scenePacketSystem;
    if (!world->GetPipeline()->GetInterface(&scenePacketSystem)) {
        ErrorLog(this).Write("Expected an installed scene packet system");
        return COM::kError;
    }

    // Install camera worldHost
    CameraHost *cameraHost;
    if (!registry->CreateClass(world, &cameraHost) || !cameraHost->Initialize() || !world->GetPipeline()->AddInterface(cameraHost)) {
        ErrorLog(this).Write("Failed to install camera worldHost");
        return COM::kError;
    }

    // Install scene camera packet batcher
    SceneCameraPacketBatcher *sceneCameraPacketBatcher;
    if (!registry->CreateClass(world, &sceneCameraPacketBatcher) || !sceneCameraPacketBatcher->Initialize(cameraHost) || !world->GetPipeline()->AddClass(sceneCameraPacketBatcher) || !scenePacketSystem->Register(sceneCameraPacketBatcher)) {
        ErrorLog(this).Write("Failed to install camera worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result SceneWorldPackage::Initialize(IScene *scene) {
    if (auto spatialScene = COM::Cast<ISpatialScene>(scene)) {
        // Install mesh proxy
        SceneMeshProxySystem *meshProxySystem;
        if (!registry->CreateClass(scene, &meshProxySystem) || !spatialScene->Register(meshProxySystem)) {
            ErrorLog(this).Write("Failed to install scene mesh proxy system");
            return COM::kError;
        }

        // Install directional light proxy
        SceneDirectionalLightProxySystem *directionalLightProxySystem;
        if (!registry->CreateClass(scene, &directionalLightProxySystem) || !spatialScene->Register(directionalLightProxySystem)) {
            ErrorLog(this).Write("Failed to install scene directional light proxy system");
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result SceneWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result SceneWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
