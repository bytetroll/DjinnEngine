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
 on 8/31/2018.
//

#include <UI/Elements/CallStack.h>
#include <UI/Elements/UILabel.h>
#include <Game/IGameSystem.h>
#include <Base/DefaultLogs.h>
#include <Data/IAssetHost.h>
#include <Data/IFontAsset.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <Async/CallGraph.h>
#include <UI/ElementComponents/UILeafTransformComponent.h>
#include <Graphics/UIDrawPacket.h>
#include <UI/ImmediateBuilder.h>

DJINN_NS2(UI);

CallStack::CallStack(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIElement(registry, outer, host) {

}

void CallStack::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&system), Error, "Expected game system to be installed");

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return;
    }

    // Default font
    renderer->font = host->CompileAssetAsync<Data::IFontAsset>("Runtime/Engine/Fonts/Akrobat-Black.otf"_path);

    callback.SetCallback(Bindff(Update));
    callback.Register(this);
}

void CallStack::Update(double delta) {
    auto graph = system->GetAsyncGraph();

    renderer->calls.Clear();

    for (USize i = 0; i < graph->GetSize(); i++) {
        auto &call = graph->Get(i);

        double actualElapsed = (call.endTimeStamp - call.startTimeStamp);

        auto name = call.name ?: call.rtti.frameName;

        if (name && actualElapsed > 0) {
            renderer->calls[name] += actualElapsed;
        }
    }
}

CallStack::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIRenderComponent(registry, outer, host) {

}

void CallStack::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    if (!font) {
        return;
    }

    ImmediateBuilder builder(transform, buffer);
    builder.SetFont(font);

    Core::String labelCache;

    double total = 0;
    float offset = 25;
    for (auto kv : calls) {
        labelCache.Flush();

        labelCache += kv.first;
        labelCache += " - ";
        labelCache += static_cast<UInt32>(kv.second * 1000000);
        labelCache += "us";

        total += kv.second;

        builder.Label(cache, {0, offset}, labelCache);

        offset += 25.0f;
    }

    builder.Label(cache, {0, 0}, Core::String("Total: ") + static_cast<UInt32>(total * 1000000) + "us", {0, 1, 1, 1});
}
