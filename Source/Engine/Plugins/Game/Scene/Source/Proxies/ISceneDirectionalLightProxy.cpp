//
// Created by Dev on 8/27/2018.
//

#include <Game/Proxies/ISceneDirectionalLightProxy.h>
#include <Game/ICameraRenderer.h>
#include <Graphics/CameraPacket.h>

DJINN_NS2(Game);

SceneDirectionalLightProxySystem::SceneDirectionalLightProxySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneProxySystem(registry, outer, host) {

}

void SceneDirectionalLightProxySystem::Build(COM::IQueryable* queryable, const Core::ArrayView<SceneProxyRequest> &requests) {
    // Must have renderer
    ICameraRenderer* renderer;
    if (!queryable->QueryInterface(&renderer)) {
        return;
    }

    // Get camera packet
    auto cameraPacket = renderer->GetPacket();

    // Get buffer
    auto buffer = cameraPacket->batch.GetPacket<Graphics::SceneDirectionalLightPacketBuffer>();

    // Handle requests
    for (auto&& request : requests) {
        auto proxy = static_cast<ISceneDirectionalLightProxy*>(request.proxy);

        // Build proxy
        proxy->Build(request.objects, buffer);
    }
}

COM::InterfaceID SceneDirectionalLightProxySystem::GetProxyInterfaceID() {
    return ISceneDirectionalLightProxy::kIID;
}
