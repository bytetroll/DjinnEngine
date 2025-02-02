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

#include <UI/Elements/CallGraph.h>
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

CallGraph::CallGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void CallGraph::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<UILeafTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&renderer->system), Error, "Expected game system to be installed");

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return;
    }

    // Default font
    renderer->font = host->CompileAssetAsync<Data::IFontAsset>("Runtime/Engine/Fonts/Akrobat-Black.otf"_path);
}

void CallGraph::Update(double delta) {
}

CallGraph::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIRenderComponent(registry, outer, host) {

}

void CallGraph::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    auto graph = system->GetAsyncGraph();

    if (!graph || graph->GetSize() < 2 || !font) {
        return;
    }

    ImmediateBuilder builder(transform, buffer);
    builder.SetFont(font);

    struct Call {
        int offset = 0;
    };

    struct Queue {
        int offset = 0;
        int maxOffset = 0;
    };

    Core::HashMap<Platform::ThreadAPI::ThreadID, Queue> queues;
    Core::HashMap<USize, Call> calls;

    double firstStamp = 1e28;
    double lastStamp = 0;

    for (USize i = 0; i < graph->GetSize(); i++) {
        auto &info = graph->Get(i);

        auto &call = calls[i];
        auto &queue = queues[info.threadID];

        if (info.parent) {
            auto& parentCall = calls[info.parent];

            call.offset = parentCall.offset + 1;
        }

        firstStamp = Math::Min(firstStamp, info.startTimeStamp);
        lastStamp = Math::Max(lastStamp, info.endTimeStamp);

        queue.maxOffset = Math::Max(queue.offset, call.offset);
    }

    double stampFactor = transform->GetDesiredSize().x / (lastStamp - firstStamp);

    {
        int offset = 0;

        for (const auto& queue : queues) {
            queue.second.offset = offset;

            builder.SetBrush(Graphics::UIBrushSolid({.5f, 0, 0, 1}));
            builder.Box(Math::Vector2(0, queue.second.offset * 27.0f - 4.0f), Math::Vector2(transform->GetDesiredSize().x, 1));

            offset += (queue.second.maxOffset + 1);
        }
    }

    for (USize i = 0; i < graph->GetSize(); i++) {
        auto& info = graph->Get(i);

        //DJINN_ASSERT(info.startTimeStamp <= firstStamp);

        auto& call = calls[i];
        auto& queue = queues[info.threadID];

        Math::Vector2 position = {
                static_cast<float>((info.startTimeStamp - firstStamp) * stampFactor),
                (queue.offset) * 27.0f + (call.offset) * 25.0f
        };

        Math::Vector2 size = {
                static_cast<float>((info.endTimeStamp - info.startTimeStamp) * stampFactor),
                20.0f
        };

        auto name = info.name ?: info.rtti.frameName;

        builder.SetDepthOffset(0);
        builder.SetPositionMode(ImmediatePositionMode::eNone);
        builder.SetBrush(Graphics::UIBrushSolid({
                                                        0.3f + (reinterpret_cast<USize>(name) % 177) / 177.0f * 0.4f,
                                                        0.3f + (reinterpret_cast<USize>(name) % 32) / 32.0f * 0.4f,
                                                        0.3f + (reinterpret_cast<USize>(name) % 65) / 65.0f * 0.4f,
                                                        1.0f
        }));
        builder.Box(position, size);

        double actualElapsed = (info.endTimeStamp - info.startTimeStamp);

        if (name && actualElapsed >= 0) {
            labelCache.Flush();

            labelCache += name;
            labelCache += " - ";
            labelCache += static_cast<UInt32>(actualElapsed * 1000000);
            labelCache += "us (";
            labelCache += static_cast<UInt32>((info.runningTime / actualElapsed) * 100);
            labelCache += "%)";

            builder.SetDepthOffset(1);
            builder.SetLocalViewport(position, size);
            builder.SetPositionMode(ImmediatePositionMode::eCenter, size);
            builder.Label(cache, position, labelCache);
            builder.ResetLocalViewport();
        }
    }
}
