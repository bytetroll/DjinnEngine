//
// Created by Dev on 8/27/2018.
//

#include <Game/Proxies/ISceneMeshProxy.h>
#include <Game/ICameraRenderer.h>
#include <Graphics/CameraPacket.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Game);

SceneMeshProxySystem::SceneMeshProxySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneProxySystem(registry, outer, host) {

}

void SceneMeshProxySystem::Build(COM::IQueryable* queryable, const Core::ArrayView<SceneProxyRequest> &requests) {
    // Must have renderer
    ICameraRenderer* renderer;
    if (!queryable->QueryInterface(&renderer)) {
        return;
    }

    // Get camera packet
    auto cameraPacket = renderer->GetPacket();

    // Get buffer
    auto buffer = cameraPacket->batch.GetPacket<Graphics::SceneMeshPacketBuffer>();

    // Handle requests
    Async::Group group;
    Async::Chunked(group, requests.Size(), [=](USize begin, USize end) {
        auto sub = buffer->PopBuffer();

        for (USize i = begin; i < end; i++) {
            auto proxy = static_cast<ISceneMeshProxy*>(requests[i].proxy);

            // Build proxy
            proxy->Build(requests[i].objects, *sub);
        }

        buffer->PushBuffer(sub);
    });
}

COM::InterfaceID SceneMeshProxySystem::GetProxyInterfaceID() {
    return ISceneMeshProxy::kIID;
}
