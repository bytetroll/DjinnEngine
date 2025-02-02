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

#include <Game/SceneStructure.h>
#include <Game/SceneOcclusion.h>
#include <Game/ISceneProxy.h>
#include <Game/SceneStructureViewport.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS2(Game);

SceneStructure::SceneStructure(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ISceneStructure(registry, outer, host) {

}

COM::Result SceneStructure::Initialize() {
    SceneOcclusion *defaultOcclusion;
    CheckMsg(registry->CreateClass(this, &defaultOcclusion), Error, "Failed to create default scene occlusion");

    // ...
    occlusion = defaultOcclusion;

    // OK
    return COM::kOK;
}

void SceneStructure::SetOcclusion(ISceneOcclusion *occlusion) {
    this->occlusion = occlusion;
}

ISceneOcclusion *SceneStructure::GetOcclusion() {
    return occlusion;
}

void SceneStructure::Register(ISceneProxy *proxy) {
    auto interface = GetInterfaceState(proxy->GetInterfaceID());

    interface->proxies.Add(proxy);
}

void SceneStructure::Deregister(ISceneProxy *proxy) {
    auto interface = GetInterfaceState(proxy->GetInterfaceID());

    interface->proxies.RemoveValue(proxy);
}

COM::Result SceneStructure::Build() {
    proxyObjectBuffer.Clear();

    // Clear all dynamic data
    for (auto &&state : interfaceStates) {
        state.dynamicRequests.Clear();
    }

    // Process all interfaces
    for (auto it : interfaceMap) {
        auto interface = it.second;

        // Process all proxies
        for (auto proxy : interface->proxies) {
            // Dynamic?
            if (proxy->GetDynamicPolicy()) {
                builder.Clear();
                proxy->BuildScene(builder);

                if (builder.objects.Size() > 0) {
                    proxyObjectBuffer.Add(builder.objects);

                    SceneProxyRequest request;
                    request.proxy = proxy;
                    request.objects = Core::ArrayView<SceneObject>(nullptr, builder.objects.Size());

                    interface->dynamicRequests.Add(request);
                }
            }
        }
    }

    // Process all interfaces
    USize offset = 0;
    for (auto it : interfaceMap) {
        auto interface = it.second;

        // Process all dynamic requests
        for (auto request : interface->dynamicRequests) {
            request.objects = Core::ArrayView(&proxyObjectBuffer[offset], request.objects.Size());
            offset += request.objects.Size();
        }
    }

    // OK
    return COM::kOK;
}

COM::Result SceneStructure::BuildViewport(COM::IQueryable *queryable, SceneStructureViewport *viewport, const Math::Matrix4 &transform) {
    // No spatial mapping, just in and out
    for (auto &&out : viewport->interfaces) {
        auto interface = GetInterfaceState(out.iid);

        //out.requests.Add(interface->staticRequests.Ptr(), interface->staticRequests.Size());
        out.requests.Add(interface->dynamicRequests);
    }

    return COM::kOK;
}

SceneStructure::InterfaceState *SceneStructure::GetInterfaceState(const COM::InterfaceID &iid) {
    auto it = interfaceMap.Find(iid);
    if (it != interfaceMap.End()) {
        return (*it).second;
    }

    auto state = &interfaceStates.Emplace();
    state->iid = iid;
    interfaceMap.Add(iid, state);
    return state;
}
