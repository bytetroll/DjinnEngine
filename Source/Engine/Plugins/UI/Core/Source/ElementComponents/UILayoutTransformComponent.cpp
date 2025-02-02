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

#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

UILayoutTransformComponent::UILayoutTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUITransformComponent(registry, outer, host) {

}

Math::Vector2 UILayoutTransformComponent::Measure() {
    if (horizontalScaling != UIScaling::eNone) {
        desiredSize.x = 0;
    }
    if (verticalScaling != UIScaling::eNone) {
        desiredSize.y = 0;
    }

    for (auto &&child : children) {
        if (horizontalScaling != UIScaling::eNone) {
            desiredSize.x = Math::Max(desiredSize.x, child->GetMargin().x + child->GetDesiredSize().x + padding.x + padding.z);
        }

        if (verticalScaling != UIScaling::eNone) {
            desiredSize.y = Math::Max(desiredSize.y, child->GetMargin().y + child->GetDesiredSize().y + padding.y + padding.w);
        }
    }

    return desiredSize;
}

void UILayoutTransformComponent::Arrange() {
    // Reset min-maxes
    desiredSizeMin = {};
    desiredSizeMax = {};

    // Max bounds
    auto max = Math::Vector2(pixelTransform.w.x, pixelTransform.w.y) + desiredSize;

    // Determine child viewport
    auto childViewport = Math::Vector4(
            Math::Vector2(Math::Max(pixelTransform.w.x, viewport.x), Math::Max(pixelTransform.w.y, viewport.y)),
            Math::Vector2(Math::Min(max.x, viewport.z), Math::Min(max.y, viewport.w))
    );

    // Update all children
    for (auto &&child : children) {
        Math::Vector2 adjustedMargin = child->GetMargin();
        Math::Vector2 adjustedPadding = {};

        // Account for horizontal alignment
        switch (child->GetHorizontalAlignment()) {
            case UIHorizontalAlignment::eLeft:
                adjustedPadding.x = padding.x;
                break;
            case UIHorizontalAlignment::eRight:
                adjustedPadding.x = -padding.z - padding.x;
                adjustedMargin.x = desiredSize.x - child->GetDesiredSize().x - child->GetMargin().x;
                break;
            case UIHorizontalAlignment::eCenter:
                adjustedMargin.x = desiredSize.x / 2.0f - child->GetDesiredSize().x / 2.0f + child->GetMargin().x;
                break;
        }

        // Account for vertical alignment
        switch (child->GetVerticalAlignment()) {
            case UIVerticalAlignment::eTop:
                adjustedPadding.y = padding.y;
                break;
            case UIVerticalAlignment::eBottom:
                adjustedPadding.y = -padding.w - padding.y;
                adjustedMargin.y = desiredSize.y - child->GetDesiredSize().y - child->GetMargin().y;
                break;
            case UIVerticalAlignment::eCenter:
                adjustedMargin.y = desiredSize.y / 2.0f - child->GetDesiredSize().y / 2.0f + child->GetMargin().y;
                break;
        }

        // ...
        auto adjustedSize = child->GetDesiredSize();

        // Account for horizontal scaling
        switch (child->GetHorizontalScaling()) {
            case UIScaling::eNone:
                break;
            case UIScaling::eFill:
                adjustedSize.x = desiredSize.x - (padding.x + padding.z) - adjustedMargin.x;
                break;
            case UIScaling::eContent:
                break;
        }

        // Account for vertical scaling
        switch (child->GetVerticalScaling()) {
            case UIScaling::eNone:
                break;
            case UIScaling::eFill:
                adjustedSize.y = desiredSize.y - (padding.y + padding.w) - adjustedMargin.y;
                break;
            case UIScaling::eContent:
                break;
        }

        // ...
        child->SetDesiredSize(adjustedSize);

        Math::Vector2 adjustedOffset = adjustedMargin + adjustedPadding;

        // Accumulate size
        desiredSizeMax = Math::Vector2::Max(desiredSizeMax, adjustedOffset + adjustedSize);

        // Final transform
        auto uiTransform = Math::Matrix4::Translate({adjustedOffset, 0}) * pixelTransform;

        // Is the child transform visible?
        child->SetTransform(uiTransform);

        // Child may not inherit the viewport
        switch (child->GetViewportMode()) {
            case UIViewportMode::eNone:
                child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                break;
            case UIViewportMode::eKeep:
                child->SetViewport(GetViewport());
                break;
            case UIViewportMode::eInherit:
                child->SetViewport(childViewport);
                break;
        }
    }
}

void UILayoutTransformComponent::AddChild(IUITransformComponent *child) {
    GetActor()->AssertReference(child->GetActor());

    children.Add(child->GetWeakPtr());
    child->SetParentReference(this);
    Bind(child->GetElement());
    child->MarkAsDirty(true);
}

void UILayoutTransformComponent::RemoveChild(IUITransformComponent *child) {
    children.RemoveValue(child->GetWeakPtr());
    child->MarkAsDirty(true);
    child->SetParentReference(nullptr);
    child->GetElement()->Detach();
}

IUITransformComponent *UILayoutTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {

    IUITransformComponent *transform = nullptr;
    for (auto &&child : children) {
        if (transform && transform->GetAbsoluteDepth() > child->GetAbsoluteDepth()) {
            continue;
        }

        if (auto ptr = child->TraceChild(point, mode)) {
            transform = ptr;
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

IUITransformComponent *UILayoutTransformComponent::FindChild(const Core::StringView &name) {
    for (auto &&child : children) {
        if (child->GetElement()->GetName() == name) {
            return child;
        }

        if (auto ptr = child->FindChild(name)) {
            return ptr;
        }
    }

    return nullptr;
}

bool UILayoutTransformComponent::OnChildDetached(IUITransformComponent *child) {
    RemoveChild(child);
    return true;
}

void UILayoutTransformComponent::OnDirty() {
    for (auto &&child : children) {
        child->MarkAsDirty();
    }
}

void UILayoutTransformComponent::StyleEvent(const ElementStyle &style) {
    IUITransformComponent::StyleEvent(style);

    padding = style.GetOrDefault("padding"_id, padding);
}

