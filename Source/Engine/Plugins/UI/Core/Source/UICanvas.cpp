//
// Created by Dev on 8/22/2018.
//

#include <UI/UICanvas.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <UI/Elements/CanvasLayout.h>
#include <Graphics/UIDrawPacket.h>
#include <Graphics/WorldPacket.h>
#include <Graphics/ScenePacket.h>
#include <Graphics/UIPacket.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPacketSystem.h>
#include <Graphics/IOutput.h>
#include <Game/IWorld.h>
#include <Game/IActorSystem.h>
#include <Graphics/UILayerPacket.h>

DJINN_NS2(UI);

UICanvas::UICanvas(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUICanvas(registry, outer, host) {

}

COM::Result UICanvas::Initialize(Game::IWorld *world, Game::ISceneState *state) {
    this->world = world;
    this->state = state;

    // Get system
    Game::IActorSystem *system;
    CheckMsg(world->GetPipeline()->GetInterface(&system), Error, "Expected actor system to be instlled");

    // Create style
    CheckMsg(registry->CreateClass(this, &style), Error, "Failed to create style");

    // Get packet system
    Graphics::IPacketSystem *packetSystem;
    CheckMsg(world->GetPipeline()->GetInterface(&packetSystem), Error, "Expected graphics packet system to be installed");
    {
        Check(packetSystem->Register(this));
    }

    // Create div
    Check(system->CreateActor(this, &canvasLayout));

    // Allow cross world referencing
    canvasLayout->SetCrossWorldReferencing(true);

    // OK
    return COM::kOK;
}

COM::Result UICanvas::Build() {
    // Ignore proxy canvas
    if (proxy) {
        return COM::kOK;
    }

    // ...
    auto transform = canvasLayout->GetTransform();

    // Scale if needed
    if (scaleOutput && output) {
        auto size = output->GetSize();
        transform->SetDesiredSize(Math::Vector2(size.width, size.height));
    }

    // Update layout
    transform->UpdateLayout(this);

    // OK
    return COM::kOK;
}

void UICanvas::Register(IUIRenderComponent *component) {
    DJINN_ASSERT(component->GetWorld() == GetWorld(), "Adding component from foreign world");

    if (proxy) {
        proxy->Register(component);
        return;
    }
    renderComponents.Add(component->GetWeakPtr());
}

void UICanvas::Deregister(IUIRenderComponent *component) {
    if (proxy) {
        proxy->Deregister(component);
        return;
    }
    renderComponents.RemoveValue(component->GetWeakPtr());
}

COM::Result UICanvas::Build(Graphics::WorldPacket *batch) {
    if (!output || proxy) {
        return COM::kOK;
    }

    // Get ui buffer packet
    auto buffer = batch->batch.GetPacket<Graphics::UIPacketBuffer>();

    // Add
    auto packet = buffer->Add();
    packet->output = output;

    // Default controller
    UILayerController controller;

    // Build default layer
    if (COM::CheckedResult result = Build(packet, controller)) {
        return result;
    }

    // OK
    return COM::kOK;
}

COM::Result UICanvas::Build(Graphics::UIPacket *packet, UILayerPass pass) {
    // Get layer buffer
    auto buffer = packet->batch.GetPacket<Graphics::UILayerPacketBuffer>();

    // Add
    auto layer = buffer->Add();
    layer->depth = pass.GetOffset();

    // Get ui draw buffer packet
    auto drawBuffer = layer->batch.GetPacket<Graphics::UIDrawPacketBuffer>();

    // Build
    for (auto &&com : renderComponents) {
        com->Build(drawBuffer);
    }

    // Build child layers
    for (auto subLayer : layers) {
        if (COM::CheckedResult result = subLayer->Build(packet, pass.Next())) {
            return result;
        }
    }

    // OK
    return COM::kOK;
}

bool UICanvas::Trace(const Math::Vector2 &position, Math::Vector2 *out) {
    *out = position;
    return true;
}

IUIElement *UICanvas::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    if (auto transform = canvasLayout->GetTransform()->TraceChild(point, mode)) {
        return transform->GetElement();
    }
    return nullptr;
}

void UICanvas::AddElement(IUIElement *element) {
    canvasLayout->AddChild(element->GetTransform());
}

void UICanvas::RemoveElement(IUIElement *element) {
    canvasLayout->RemoveChild(element->GetTransform());
}

void UICanvas::Register(IUILayer *layer) {
    layers.Add(layer);
}

void UICanvas::Deregister(IUILayer *layer) {
    layers.RemoveValue(layer);
}

Game::IWorld *UICanvas::GetWorld() {
    return world;
}

Game::ISceneState *UICanvas::GetState() {
    return state;
}

void UICanvas::SetProxy(IUICanvas *canvas) {
    proxy = canvas;
    canvas->AddElement(canvasLayout);
}

Style *UICanvas::GetStyle() {
    if (proxy) {
        return proxy->GetStyle();
    }
    return style;
}
