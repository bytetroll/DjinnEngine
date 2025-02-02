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
 on 8/20/2018.
//

#include <UI/ElementComponents/UILayerTransformComponent.h>
#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Graphics/UIPacket.h>
#include <Graphics/UILayerPacket.h>
#include <Graphics/UIDrawPacket.h>

DJINN_NS2(UI);

UILayerTransformComponent::UILayerTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUITransformComponent(registry, outer, host) {

}

Math::Vector2 UILayerTransformComponent::Measure() {
    if (horizontalScaling != UIScaling::eNone) {
        desiredSize.x = 0;
    }
    if (verticalScaling != UIScaling::eNone) {
        desiredSize.y = 0;
    }

    for (auto &&info : layers) {
        if (horizontalScaling != UIScaling::eNone) {
            desiredSize.x = Math::Max(desiredSize.x, info.transform->GetMargin().x + info.transform->GetDesiredSize().x + padding.x + padding.z);
        }

        if (verticalScaling != UIScaling::eNone) {
            desiredSize.y = Math::Max(desiredSize.y, info.transform->GetMargin().y + info.transform->GetDesiredSize().y + padding.y + padding.w);
        }
    }

    return desiredSize;
}

void UILayerTransformComponent::Arrange() {
    // Max bounds
    auto max = Math::Vector2(pixelTransform.w.x, pixelTransform.w.y) + desiredSize;

    // Determine child viewport
    auto childViewport = Math::Vector4(
            Math::Vector2(Math::Max(pixelTransform.w.x, viewport.x), Math::Max(pixelTransform.w.y, viewport.y)),
            Math::Vector2(Math::Min(max.x, viewport.z), Math::Min(max.y, viewport.w))
    );

    // Update all children
    for (auto &&info : layers) {
        Math::Vector2 adjustedMargin = info.transform->GetMargin();
        Math::Vector2 adjustedPadding = {};

        // Account for horizontal alignment
        switch (info.transform->GetHorizontalAlignment()) {
            case UIHorizontalAlignment::eLeft:
                adjustedPadding.x = padding.x;
                break;
            case UIHorizontalAlignment::eRight:
                adjustedPadding.x = -padding.z - padding.x;
                adjustedMargin.x = desiredSize.x - info.transform->GetDesiredSize().x - info.transform->GetMargin().x;
                break;
            case UIHorizontalAlignment::eCenter:
                adjustedMargin.x = desiredSize.x / 2.0f - info.transform->GetDesiredSize().x / 2.0f + info.transform->GetMargin().x;
                break;
        }

        // Account for vertical alignment
        switch (info.transform->GetVerticalAlignment()) {
            case UIVerticalAlignment::eTop:
                adjustedPadding.y = padding.y;
                break;
            case UIVerticalAlignment::eBottom:
                adjustedPadding.y = -padding.w - padding.y;
                adjustedMargin.y = desiredSize.y - info.transform->GetDesiredSize().y - info.transform->GetMargin().y;
                break;
            case UIVerticalAlignment::eCenter:
                adjustedMargin.y = desiredSize.y / 2.0f - info.transform->GetDesiredSize().y / 2.0f + info.transform->GetMargin().y;
                break;
        }

        // ...
        auto adjustedSize = info.transform->GetDesiredSize();

        // Account for horizontal scaling
        switch (info.transform->GetHorizontalScaling()) {
            case UIScaling::eNone:
                break;
            case UIScaling::eFill:
                adjustedSize.x = desiredSize.x - (padding.x + padding.z) - adjustedMargin.x;
                break;
            case UIScaling::eContent:
                break;
        }

        // Account for vertical scaling
        switch (info.transform->GetVerticalScaling()) {
            case UIScaling::eNone:
                break;
            case UIScaling::eFill:
                adjustedSize.y = desiredSize.y - (padding.y + padding.w) - adjustedMargin.y;
                break;
            case UIScaling::eContent:
                break;
        }

        // ...
        info.transform->SetDesiredSize(adjustedSize);

        // Final transform
        auto uiTransform = Math::Matrix4::Translate({adjustedMargin + adjustedPadding, 0}) * pixelTransform;

        // Is the child transform visible?
        info.transform->SetTransform(uiTransform);

        // Child may not inherit the viewport
        switch (info.transform->GetViewportMode()) {
            case UIViewportMode::eNone:
                info.transform->SetViewport({-1e6, -1e6, 1e6, 1e6});
                break;
            case UIViewportMode::eKeep:
                info.transform->SetViewport(GetViewport());
                break;
            case UIViewportMode::eInherit:
                info.transform->SetViewport(childViewport);
                break;
        }
    }
}

IUITransformComponent *UILayerTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    IUITransformComponent *transform = nullptr;
    for (Int64 i = static_cast<Int64>(layers.Size()) - 1; i >= 0; i--) {
        if (auto ptr = layers[static_cast<USize>(i)].transform->TraceChild(point, mode)) {
            return ptr;
        }
    }

    if (!transform) {
        if (isTraceFallthrough && mode == TransformTraceMode::eInput) {
            return nullptr;
        }

        return IsPointInside(point) ? this : nullptr;
    }

    return transform;
}

IUITransformComponent *UILayerTransformComponent::FindChild(const Core::StringView &name) {
    for (auto &&info : layers) {
        if (info.transform->GetElement()->GetName() == name) {
            return info.transform;
        }

        if (auto ptr = info.transform->FindChild(name)) {
            return ptr;
        }
    }

    return nullptr;
}

void UILayerTransformComponent::Register(IUIRenderComponent *component) {
    renderComponents.Add(component->GetWeakPtr());
}

void UILayerTransformComponent::Deregister(IUIRenderComponent *component) {
    renderComponents.RemoveValue(component->GetWeakPtr());
}

COM::Result UILayerTransformComponent::Build(Graphics::UIPacket *packet, UILayerPass pass) {
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

    // Build lose layers
    for (auto layer : loseLayers) {
        if (COM::CheckedResult result = layer->Build(packet, pass.Next())) {
            return result;
        }
    }

    // Build child layers
    for (auto info : layers) {
        if (info.layer) {
            if (COM::CheckedResult result = info.layer->Build(packet, pass.Next())) {
                return result;
            }
        }
    }

    // OK
    return COM::kOK;
}

void UILayerTransformComponent::AddChild(IUITransformComponent *transform, IUILayer *layer) {
    UILayerTransformInfo info;
    info.transform = transform;
    info.layer = layer;
    layers.Add(info);

    transform->SetParentReference(this);
    Bind(transform->GetElement());
    transform->MarkAsDirty(true);

    SortLayers();
}

void UILayerTransformComponent::RemoveChild(IUITransformComponent *transform) {
    layers.RemoveAll([&](UILayerTransformInfo& info) {
        return info.transform == transform;
    });

    transform->MarkAsDirty(true);
    transform->SetParentReference(nullptr);
    transform->GetElement()->Detach();
}

void UILayerTransformComponent::SetPriority(IUITransformComponent *transform, USize priority) {
    for (auto &&info : layers) {
        if (info.transform == transform) {
            info.priority = priority;
            break;
        }
    }

    SortLayers();
}

USize UILayerTransformComponent::GetPriority(IUITransformComponent *transform) {
    for (auto &&info : layers) {
        if (info.transform == transform) {
            return info.priority;
        }
    }

    return 0;
}

void UILayerTransformComponent::SortLayers() {
    layers.Sort([&](const UILayerTransformInfo& lhs, const UILayerTransformInfo& rhs) { return lhs.priority < rhs.priority; });
}

void UILayerTransformComponent::OnDirty() {
    for (auto &&info : layers) {
        if (info.transform) {
            info.transform->MarkAsDirty();
        }
    }
}

IUILayer *UILayerTransformComponent::RequestLayer(IUILayer *layer) {
    if (parentLayer) {
        layer->Deregister(this);
    }
    layer->Register(this);
    parentLayer = layer;
    return this;
}

void UILayerTransformComponent::Register(IUILayer *layer) {
    if (!layers.Any([&](const UILayerTransformInfo& info) { return info.layer == layer; })) {
        loseLayers.Add(layer);
    }
}

void UILayerTransformComponent::Deregister(IUILayer *layer) {
    loseLayers.RemoveValue(layer);
}

bool UILayerTransformComponent::OnChildDetached(IUITransformComponent *child) {
    bool found = false;
    layers.RemoveAll([&](UILayerTransformInfo& info) {
        if (info.transform == child) {
            found = true;
            return true;
        }
        return false;
    });
    return found;
}

