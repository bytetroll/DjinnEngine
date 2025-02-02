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

#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

UIStackTransformComponent::UIStackTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUITransformComponent(registry, outer, host) {
    SetChildDependent(true);
}

Math::Vector2 UIStackTransformComponent::Measure() {
    stackDesiredSize = {};
    fillCount = 0;

    if (GetElement()->GetName() == "tits") {
        bool c = false;
    }

    for (auto &&child : children) {
        switch (mode) {
            case UIStackMode::eHorizontal: {
                if (child->GetHorizontalScaling() == UIScaling::eFill) {
                    fillCount++;
                }
                break;
            }
            case UIStackMode::eVertical: {
                if (child->GetVerticalScaling() == UIScaling::eFill) {
                    fillCount++;
                }
                break;
            }
            case UIStackMode::eScanGrid: {
                break;
            }
        }

        if ((mode == UIStackMode::eVertical || child->GetHorizontalAlignment() == UIHorizontalAlignment::eLeft) && (mode == UIStackMode::eHorizontal || child->GetVerticalAlignment() == UIVerticalAlignment::eTop)) {
            switch (mode) {
                case UIStackMode::eHorizontal: {
                    if (child->GetHorizontalScaling() != UIScaling::eFill) {
                        stackDesiredSize.x += child->GetDesiredSize().x + padding;
                        stackDesiredSize.y = Math::Max(stackDesiredSize.y, child->GetDesiredSize().y, 0);
                    }
                    break;
                }
                case UIStackMode::eVertical: {
                    if (child->GetVerticalScaling() != UIScaling::eFill) {
                        stackDesiredSize.x = Math::Max(stackDesiredSize.x, child->GetDesiredSize().x, 0);
                        stackDesiredSize.y += child->GetDesiredSize().y + padding;
                    }
                    break;
                }
                case UIStackMode::eScanGrid: {
                    break;
                }
            }
        }
    }

    if (GetHorizontalScaling() == UIScaling::eContent) {
        desiredSize.x = stackDesiredSize.x;
    }

    if (GetVerticalScaling() == UIScaling::eContent) {
        desiredSize.y = stackDesiredSize.y;
    }

    return stackDesiredSize;
}

void UIStackTransformComponent::Arrange() {
    // Reset min-maxes
    desiredSizeMin = {};
    desiredSizeMax = {};

    switch (mode) {
        case UIStackMode::eHorizontal: {
            float offset = 0;

            float fillSize = fillCount ? (Math::Max(desiredSize.x - stackDesiredSize.x, 0.0f) / fillCount) : 0;

            for (auto &child : children) {
                Math::Vector2 begin = Math::Vector2(offset, 0);

                Math::Vector2 adjustedMargin = {offset, 0};

                Math::Vector2 adjustedSize = child->GetDesiredSize();
                switch (child->GetVerticalScaling()) {
                    case UIScaling::eNone:
                        break;
                    case UIScaling::eFill:
                        adjustedSize.y = GetDesiredSize().y;
                        break;
                    case UIScaling::eContent:
                        break;
                }

                if (child->GetHorizontalScaling() == UIScaling::eFill) {
                    adjustedSize.x = fillSize;
                    offset += fillSize;
                } else {
                    offset += child->GetDesiredSize().x + padding;
                }

                switch (child->GetVerticalAlignment()) {
                    case UIVerticalAlignment::eTop: {
                        adjustedMargin.y = child->GetMargin().y;
                        break;
                    }
                    case UIVerticalAlignment::eCenter: {
                        adjustedMargin.y = desiredSize.y * 0.5f - child->GetDesiredSize().y * 0.5f + child->GetMargin().y;
                        break;
                    }
                    case UIVerticalAlignment::eBottom: {
                        adjustedMargin.y = desiredSize.y - child->GetMargin().y - adjustedSize.y;
                        break;
                    }
                }

                child->SetDesiredSize(adjustedSize);

                // Accumulate size
                desiredSizeMax = Math::Vector2::Max(desiredSizeMax, adjustedMargin + child->GetDesiredSize());

                // Final transform
                child->SetTransform(Math::Matrix4::Translate({adjustedMargin, 0}) * pixelTransform);

                // Set the viewport
                switch (child->GetViewportMode()) {
                    case UIViewportMode::eNone:
                        child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                        break;
                    case UIViewportMode::eKeep:
                        child->SetViewport(GetViewport());
                        break;
                    case UIViewportMode::eInherit:
                        child->SetLocalViewport({begin, Math::Vector2(offset - padding, GetDesiredSize().y)}, pixelTransform, viewport);
                        break;
                }
            }
            break;
        }
        case UIStackMode::eVertical: {
            float offset = 0;

            float fillSize = fillCount ? (Math::Max(desiredSize.y - stackDesiredSize.y, 0.0f) / fillCount) : 0;

            for (auto &child : children) {
                Math::Vector2 begin = Math::Vector2(0, offset);

                Math::Vector2 adjustedMargin = {0, offset};

                Math::Vector2 adjustedSize = child->GetDesiredSize();
                switch (child->GetHorizontalScaling()) {
                    case UIScaling::eNone:
                        break;
                    case UIScaling::eFill:
                        adjustedSize.x = GetDesiredSize().x;
                        break;
                    case UIScaling::eContent:
                        break;
                }

                if (child->GetVerticalScaling() == UIScaling::eFill) {
                    adjustedSize.y = fillSize;
                    offset += fillSize;
                } else {
                    offset += child->GetDesiredSize().y + padding;
                }

                switch (child->GetHorizontalAlignment()) {
                    case UIHorizontalAlignment::eLeft: {
                        adjustedMargin.x = child->GetMargin().x;
                        break;
                    }
                    case UIHorizontalAlignment::eCenter: {
                        adjustedMargin.x = desiredSize.x * 0.5f - child->GetDesiredSize().x * 0.5f + child->GetMargin().x;
                        break;
                    }
                    case UIHorizontalAlignment::eRight: {
                        adjustedMargin.x = desiredSize.x - child->GetMargin().x - adjustedSize.x;
                        break;
                    }
                }

                child->SetDesiredSize(adjustedSize);

                // Accumulate size
                desiredSizeMax = Math::Vector2::Max(desiredSizeMax, adjustedMargin + child->GetDesiredSize());

                // Final transform
                child->SetTransform(Math::Matrix4::Translate({adjustedMargin, 0}) * pixelTransform);

                // Set the viewport
                switch (child->GetViewportMode()) {
                    case UIViewportMode::eNone:
                        child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                        break;
                    case UIViewportMode::eKeep:
                        child->SetViewport(GetViewport());
                        break;
                    case UIViewportMode::eInherit: {
                        // Determine child viewport
                        child->SetLocalViewport({begin, Math::Vector2(GetDesiredSize().x, offset - padding)}, pixelTransform, viewport);
                        break;
                    }
                }
            }
            break;
        }
        case UIStackMode::eScanGrid: {
            Math::Vector2 offset;

            float width = desiredSize.x;
            float maxRowY = 0;

            for (auto &child : children) {
                // Scangrid only cares about left & top leaning elements
                if (child->GetHorizontalAlignment() == UIHorizontalAlignment::eLeft && child->GetVerticalAlignment() == UIVerticalAlignment::eTop) {
                    maxRowY = Math::Max(maxRowY, child->GetMargin().y + child->GetDesiredSize().y);

                    // Does the child exceed the width? i.e. new row
                    // No point in exceeding the current row
                    if (offset.x > 0 && offset.x + child->GetDesiredSize().x + child->GetMargin().x > width) {
                        offset.x = 0;
                        offset.y += maxRowY + padding;
                        maxRowY = 0;
                    }

                    Math::Vector2 adjustedMargin = offset + Math::Vector2(0, child->GetMargin().y);

                    // Accumulate size
                    desiredSizeMax = Math::Vector2::Max(desiredSizeMax, adjustedMargin + child->GetDesiredSize());

                    // Final transform
                    child->SetTransform(Math::Matrix4::Translate({adjustedMargin, 0}) * pixelTransform);

                    // Set the viewport
                    switch (child->GetViewportMode()) {
                        case UIViewportMode::eNone:
                            child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                            break;
                        case UIViewportMode::eKeep:
                            child->SetViewport(GetViewport());
                            break;
                        case UIViewportMode::eInherit: {
                            // Determine child viewport
                            child->SetLocalViewport({offset, GetDesiredSize()}, pixelTransform, viewport);
                            break;
                        }
                    }

                    // Offset the next child
                    offset.x += child->GetMargin().x + child->GetDesiredSize().x + padding;
                } else {
                    child->SetTransform(Math::Matrix4::Translate({child->GetMargin(), 0}) * pixelTransform);

                    // Set the viewport
                    switch (child->GetViewportMode()) {
                        case UIViewportMode::eNone:
                            child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                            break;
                        case UIViewportMode::eKeep:
                            child->SetViewport(GetViewport());
                            break;
                        case UIViewportMode::eInherit: {
                            // Determine child viewport
                            child->SetLocalViewport({child->GetMargin(), GetDesiredSize()}, pixelTransform, viewport);
                            break;
                        }
                    }
                }
            }

            break;
        }
    }
}

void UIStackTransformComponent::AddChild(IUITransformComponent *child) {
    GetActor()->AssertReference(child->GetActor());

    children.Add(child->GetWeakPtr());
    child->SetParentReference(this);
    Bind(child->GetElement());
    child->MarkAsDirty(true);
}

void UIStackTransformComponent::RemoveChild(IUITransformComponent *child) {
    children.RemoveValue(child->GetWeakPtr());
    child->MarkAsDirty(true);
    child->SetParentReference(nullptr);
    child->GetElement()->Detach();
}

IUITransformComponent *UIStackTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    IUITransformComponent *transform = nullptr;
    for (auto &&child : children) {
        if (transform && transform->GetAbsoluteDepth() > child->GetAbsoluteDepth()) {
            //continue;
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

IUITransformComponent *UIStackTransformComponent::FindChild(const Core::StringView &name) {
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

bool UIStackTransformComponent::OnChildDetached(IUITransformComponent *child) {
    RemoveChild(child);
    return true;
}

void UIStackTransformComponent::OnDirty() {
    for (auto &&child : children) {
        child->MarkAsDirty();
    }
}
