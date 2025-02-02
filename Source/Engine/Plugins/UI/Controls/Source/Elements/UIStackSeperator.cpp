//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UIStackSeperator.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/Elements/UIImage.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/ElementComponents/UIInputComponent.h>

DJINN_NS2(UI);

StackSeperator::StackSeperator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void StackSeperator::StaticInitialize() {
    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    auto layout = CreateComponent<UIStackTransformComponent>();
    layout->SetDesiredSize({5, 5});

    // Create image
    UIImage *img;
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &img));

        img->SetTexture(host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/Seperator.png"_path));
        img->SetMarginGeometry({1.5f, 1.5f});
        img->SetColor({0.4f, 0.4f, 0.4f, 1});

        // Configure transform
        auto transform = img->GetTransform();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        transform->SetTraceFallthrough(true);
        layout->AddChild(transform);
    }

    // Configure input
    auto input = CreateComponent<UIInputComponent>();
    {
        input->BindDragged(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
            auto leftT = left->GetTransform();
            auto rightT = right->GetTransform();

            switch (mode) {
                case UIStackMode::eHorizontal: {
                    if (event.delta.x < 0) {
                        float dist = leftT->GetDesiredSize().x - Math::SaturateNegative(leftT->GetDesiredSize().x + event.delta.x);
                        leftT->SetDesiredSize(leftT->GetDesiredSize() - Math::Vector2(dist, 0));
                        rightT->SetDesiredSize(rightT->GetDesiredSize() + Math::Vector2(dist, 0));
                    } else {
                        float dist = rightT->GetDesiredSize().x - Math::SaturateNegative(rightT->GetDesiredSize().x - event.delta.x);
                        leftT->SetDesiredSize(leftT->GetDesiredSize() + Math::Vector2(dist, 0));
                        rightT->SetDesiredSize(rightT->GetDesiredSize() - Math::Vector2(dist, 0));
                    }
                    break;
                }
                case UIStackMode::eVertical: {
                    if (event.delta.y < 0) {
                        float dist = leftT->GetDesiredSize().y - Math::SaturateNegative(leftT->GetDesiredSize().y + event.delta.y);
                        leftT->SetDesiredSize(leftT->GetDesiredSize() - Math::Vector2(0, dist));
                        rightT->SetDesiredSize(rightT->GetDesiredSize() + Math::Vector2(0, dist));
                    } else {
                        float dist = rightT->GetDesiredSize().y - Math::SaturateNegative(rightT->GetDesiredSize().y - event.delta.y);
                        leftT->SetDesiredSize(leftT->GetDesiredSize() + Math::Vector2(0, dist));
                        rightT->SetDesiredSize(rightT->GetDesiredSize() - Math::Vector2(0, dist));
                    }
                    break;
                }
                case UIStackMode::eScanGrid: {
                    break;
                }
            }
        });
    }

    SetTransform(layout);
}

void StackSeperator::SetMode(UIStackMode mode) {
    this->mode = mode;

    auto transform = GetTransform();

    switch (mode) {
        case UIStackMode::eHorizontal: {
            transform->SetScaling(UIScaling::eNone, UIScaling::eFill);
            break;
        }
        case UIStackMode::eVertical: {
            transform->SetScaling(UIScaling::eFill, UIScaling::eNone);
            break;
        }
        case UIStackMode::eScanGrid: {
            break;
        }
    }
}

void StackSeperator::SetElements(IUIElement *left, IUIElement *right) {
    this->left = left;
    this->right = right;
}
