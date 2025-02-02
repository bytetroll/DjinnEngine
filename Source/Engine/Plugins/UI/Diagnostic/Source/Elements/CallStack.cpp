//
// Created by Dev on 8/31/2018.
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
