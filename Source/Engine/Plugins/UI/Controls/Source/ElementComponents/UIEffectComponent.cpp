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
 on 8/23/2018.
//

#include <UI/ElementComponents/UIEffectComponent.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/IUIElement.h>
#include <Math/Vector2.h>

DJINN_NS2(UI);

UIEffectComponent::UIEffectComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIElementComponent(registry, outer, host) {

}

void UIEffectComponent::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
}

void UIEffectComponent::Update(double delta) {
    Math::Vector2 position;
    Math::Quaternion rotation;
    Math::Vector2 scale = {1, 1};

    // Process all effects
    for (Int64 i = effectDatas.Size() - 1; i >= 0; i--) {
        auto &data = effectDatas[i];
        switch (data.effect.type) {
            case UIEffectType::eClick: {
                scale *= static_cast<float>(1.0f - Math::Sin(data.elapsed / data.effect.duration * Math::kPI) * 0.05f * data.effect.data.strength);
                break;
            }
            case UIEffectType::eBubble: {
                scale *= static_cast<float>((Math::Cos(data.elapsed * 20.f) * data.effect.data.strength * 0.5f) / (Math::Pow(data.elapsed + 1.0f, 7.0f) * 2.0f) + 1);
                break;
            }
            case UIEffectType::eWiggle: {
                rotation *= Math::Quaternion(Math::Vector3(0, 0, 1), static_cast<float>((Math::Cos(data.elapsed * 20.f) * data.effect.data.strength * 0.5f) / (Math::Pow(data.elapsed + 1.0f, 7.0f) * 2.0f)) * (Math::kPI * 0.5f));
                break;
            }
            case UIEffectType::eRotate: {
                rotation *= Math::Quaternion(Math::Vector3(0, 0, 1), data.elapsed * Math::kPI * data.effect.data.rotateSpeed);
                break;
            }
            case UIEffectType::eStretch: {
                scale *= Math::Vector2(
                        static_cast<float>((Math::Cos(data.elapsed * 20.f) * data.effect.data.strength * 0.5f) / (Math::Pow(data.elapsed + 1.0f, 7.0f) * 2.0f) + 1),
                        static_cast<float>((Math::Sin(data.elapsed * 20.f) * data.effect.data.strength * 0.5f) / (Math::Pow(data.elapsed + 1.0f, 7.0f) * 2.0f) + 1)
                );
                break;
            }
        }

        // End of effect?
        data.elapsed += delta;
        if (data.effect.duration != 0 && data.elapsed > data.effect.duration) {
            effectDatas.Remove(i);
        }
    }

    // Transform
    auto transform = GetElement()->GetTransform();

    // Apply
    transform->SetUserScale(scale);
    transform->SetUserTranslation(position);
    transform->SetUserRotation(rotation);

    // Deregister this if empty
    if (effectDatas.Size() == 0) {
        transform->ResetUserTransform();
        callback.Deregister();
    }
}
