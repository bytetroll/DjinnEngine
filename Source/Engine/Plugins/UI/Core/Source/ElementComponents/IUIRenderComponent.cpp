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
 on 8/22/2018.
//

#include <UI/ElementComponents/IUIRenderComponent.h>
#include <UI/IUILayer.h>

DJINN_NS2(UI);

void IUIRenderComponent::SetRenderPolicy(UIRenderPolicy policy) {
    this->policy = policy;

    switch (policy) {
        case UIRenderPolicy::eAlways: {
            break;
        }
        case UIRenderPolicy::eTransformChanged: {
            break;
        }
    }
}

void IUIRenderComponent::Showed(IUILayer *layer) {
    registeredLayer = layer;
    layer->Register(this);
}

void IUIRenderComponent::Hidden(IUILayer *layer) {
    registeredLayer = nullptr;
    layer->Deregister(this);
}

void IUIRenderComponent::Destroyed() {
    if (registeredLayer) {
        registeredLayer->Deregister(this);
    }
}
