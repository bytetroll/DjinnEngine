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
