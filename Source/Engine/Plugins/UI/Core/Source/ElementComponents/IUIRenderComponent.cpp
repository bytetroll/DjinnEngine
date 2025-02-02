//
// Created by Dev on 8/22/2018.
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
