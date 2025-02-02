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

#include <UI/ElementComponents/UILeafTransformComponent.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(UI);

UILeafTransformComponent::UILeafTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUITransformComponent(registry, outer, host) {

}

Math::Vector2 UILeafTransformComponent::Measure() {
    return desiredSize;
}

void UILeafTransformComponent::Arrange() {
    if (!parent) {
        ErrorLog(this).Write("Leaf without parent");
    }
}

IUITransformComponent *UILeafTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    if (isTraceFallthrough && mode == TransformTraceMode::eInput) {
        return nullptr;
    }

    return IsPointInside(point) ? this : nullptr;
}

IUITransformComponent *UILeafTransformComponent::FindChild(const Core::StringView &name) {
    return nullptr;
}

bool UILeafTransformComponent::OnChildDetached(IUITransformComponent *child) {
    return false;
}


