//
// Created by Dev on 8/20/2018.
//

#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/IUIElement.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

void IUITransformComponent::UpdateLayout(IUILayer* layer) {
    // Cache all
    PrePass(layer);

    // Passes
    StylePass();
    MeasurePass();
    ModelPass();
    ArrangePass();

    // ...
    PostPass();
}

void IUITransformComponent::PrePass(IUILayer* layer) {
    auto element = GetElement();

    if (!this->layer || this->layer != layer) {
        SetLayer(layer = RequestLayer(layer));
    }

    if (element->NeedsStyling()) {
        element->ApplyStyle();
    }

    if (!parent) {
        zAbsolute = zOffset;
    } else {
        zAbsolute = parent->zAbsolute + parent->zOffset + 1;
    }

    // Cache children
    bool contributingChild = false;
    for (auto &&child : dirtyChildren) {
        if (child) {
            if (child->contentMode == UIContentMode::eInclude) {
                contributingChild = true;
            }
            child->PrePass(layer);
        }
    }

    // Is this dirty?
    if (contributingChild && IsChildDependent()) {
        isDirty = true;
    }

    // Cache this
    if (element->NeedsCaching()) {
        // Attempt to cache
        if ((cacheResult = element->Cache())) {
            element->MarkAsCached();
        }
    }
}

void IUITransformComponent::StylePass() {
    auto element = GetElement();

    // Cache children
    for (auto &&child : dirtyChildren) {
        if (child) {
            child->StylePass();
        }
    }

    // Style this
    if (element->NeedsStyling()) {
        element->ApplyStyle();
    }
}

bool IUITransformComponent::PostPass() {
    // Are we in a hidden state or is the parent invisible?
    if (isHidden || (parent && !parent->isVisible)) {
        isVisible = false;
    } else {
        isVisible = !(
                ((pixelTransform.w.x > viewport.z) || (pixelTransform.w.x + desiredSize.x < viewport.x)) ||
                ((pixelTransform.w.y > viewport.w) || (pixelTransform.w.y + desiredSize.y < viewport.y))
        );
    }

    bool ok = cacheResult;
    dirtyChildren.RemoveAll([&](auto &&child) {
        if (child) {
            child->updated.Signal(this);

            if (!child->PostPass()) {
                ok = false;
                return false;
            }
            child->isAdded = false;
            return true;
        }
        return true;
    });

    if (ok) {
        isDirty = false;
    }

    if (wasPreviouslyVisible != isVisible) {
        auto element = GetElement();
        if (isVisible) {
            element->Showed(layer);
            for (auto &&comp : GetElement()->GetComponents()) {
                if (auto elementCom = COM::Cast<IUIElementComponent>(comp)) {
                    elementCom->Showed(layer);
                }
            }
        } else {
            GetElement()->Hidden(layer);
            for (auto &&comp : GetElement()->GetComponents()) {
                if (auto elementCom = COM::Cast<IUIElementComponent>(comp)) {
                    elementCom->Hidden(layer);
                }
            }
        }

        wasPreviouslyVisible = isVisible;
    }

    isChildDirty = dirtyChildren.Size() != 0;

    return ok;
}

void IUITransformComponent::MeasurePass() {
    // Measure children
    for (auto &&child : dirtyChildren) {
        if (child) {
            child->MeasurePass();
        }
    }

    // Measure this
    if (isDirty) {
        measuredSize = Measure();
    }
}

void IUITransformComponent::ModelPass() {
    // Model this
    if (isDirty) {
        Model();
    }

    // Model children
    for (auto &&child : dirtyChildren) {
        if (child) {
            child->ModelPass();
        }
    }
}

void IUITransformComponent::ArrangePass() {
    // Arrange this
    if (isDirty) {
        Arrange();
    }

    // Arrange children
    for (auto &&child : dirtyChildren) {
        if (child) {
            child->ArrangePass();
        }
    }
}

void IUITransformComponent::SetParentReference(IUITransformComponent *com) {
    if (com) {
        parent = com->GetWeakPtr();
    } else {
        parent = nullptr;
    }
}

void IUITransformComponent::StyleEvent(const ElementStyle &style) {
    horizontalAlignment = style.ConvertOrDefault("horizontal-alignment"_id, horizontalAlignment);
    verticalAlignment = style.ConvertOrDefault("vertical-alignment"_id, verticalAlignment);

    horizontalScaling = style.ConvertOrDefault("horizontal-scaling"_id, horizontalScaling);
    verticalScaling = style.ConvertOrDefault("vertical-scaling"_id, verticalScaling);

    contentMode = style.ConvertOrDefault("content-mode"_id, contentMode);

    margin = style.GetOrDefault("margin"_id, margin);

    desiredSize = style.GetOrDefault("desired-size"_id, desiredSize);

    zOffset = style.GetOrDefault("z-offset"_id, zOffset);
}

bool IUITransformComponent::IsPointInside(const Math::Vector2 &point) {
    auto halfSize = desiredSize * 0.5f;
    auto projPoint = Math::Quaternion::Transform(userRotation.Inversed(), Math::Vector3(point - halfSize - actualTranslation, 0)).XY() + actualTranslation + halfSize;

    auto begin = actualTranslation + halfSize - halfSize * userScale;
    auto end = actualTranslation + halfSize + halfSize * userScale;

    begin.x = Math::Max(begin.x, GetViewport().x);
    begin.y = Math::Max(begin.y, GetViewport().y);
    end.x = Math::Min(end.x, GetViewport().z);
    end.y = Math::Min(end.y, GetViewport().w);

    if (projPoint.x > begin.x && projPoint.y > begin.y && projPoint.x < end.x && projPoint.y < end.y) {
        return true;
    }

    return false;
}

void IUITransformComponent::SetTransform(const Math::Matrix4 &transform) {
    auto userTransform = (
                                 (Math::Matrix4::Translate({desiredSize / -2, 0}) * (
                                         Math::Matrix4::Scale({userScale, 1}) *
                                         Math::Matrix4::Rotate(userRotation)
                                 ) * Math::Matrix4::Translate({desiredSize / 2, 0}))
                         ) * Math::Matrix4::Translate({userTranslation, 0});

    actualTranslation = {transform.w.x, transform.w.y};

    pixelTransform = userTransform * transform;
    pixelTransform.w.z += static_cast<float>(GetDepthOffset() + 1);

    // Calculate bounding box points
    Math::Vector3 points[] = {
            Math::Matrix4::TransformPosition(pixelTransform, {0, 0, 0}),
            Math::Matrix4::TransformPosition(pixelTransform, {desiredSize.x, 0, 0}),
            Math::Matrix4::TransformPosition(pixelTransform, {0, desiredSize.y, 0}),
            Math::Matrix4::TransformPosition(pixelTransform, {desiredSize.x, desiredSize.y, 0})
    };

    // Determine min / max
    desiredSizeMin = Math::Vector3::Min(points[0], points[1], points[2], points[3]).XY();
    desiredSizeMax = Math::Vector3::Max(points[0], points[1], points[2], points[3]).XY();

    // ...
    MarkAsDirty();
}

void IUITransformComponent::SetTraceElement(IUIElement *element) {
    traceElement = element->GetWeakPtr();
}

void IUITransformComponent::Destroyed() {
    if (parent) {
        parent->OnChildDetached(this);
    }
}

void IUITransformComponent::Move(IUITransformComponent *dest) {
    dest->horizontalAlignment = horizontalAlignment;
    dest->verticalAlignment = verticalAlignment;
    dest->horizontalScaling = horizontalScaling;
    dest->verticalScaling = verticalScaling;
    dest->contentMode = contentMode;
    dest->viewportMode = viewportMode;
    dest->margin = margin;
    dest->desiredSize = desiredSize;
    dest->measuredSize = measuredSize;
    dest->viewport = viewport;
    dest->zOffset = zOffset;
    dest->zAbsolute = zAbsolute;
    dest->traceElement = traceElement;
    dest->isTraceFallthrough = isTraceFallthrough;
    dest->userTranslation = userTranslation;
    dest->userRotation = userRotation;
    dest->userScale = userScale;
    dest->actualTranslation = actualTranslation;
    dest->pixelTransform = pixelTransform;

    dest->MarkAsDirty();
}

void IUITransformComponent::SetViewport(const Math::Vector4 &outerViewport) {
    Math::Vector4 _viewport = {
            Math::Max(desiredSizeMin.x, outerViewport.x),
            Math::Max(desiredSizeMin.y, outerViewport.y),
            Math::Min(desiredSizeMax.x, outerViewport.z),
            Math::Min(desiredSizeMax.y, outerViewport.w)
    };
    if (_viewport != viewport) {
        this->viewport = _viewport;
        MarkAsDirty();
    }
}

void IUITransformComponent::SetDesiredSize(const Math::Vector2 &_desiredSize) {
    if (_desiredSize != desiredSize) {
        //InfoLog(this).Write("From [", this->desiredSize.x, ", ", this->desiredSize.y, "] to [", desiredSize.x, ", ", desiredSize.y, "]");
        desiredSize = _desiredSize;
        MarkAsDirty();
    }
}

void IUITransformComponent::SetLayer(IUILayer *layer) {
    this->layer = layer;
}

IUILayer *IUITransformComponent::RequestLayer(IUILayer *layer) {
    return layer;
}
