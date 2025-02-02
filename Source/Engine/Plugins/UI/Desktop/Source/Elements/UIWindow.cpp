//
// Created by Dev on 8/31/2018.
//

#include <UI/Elements/UIWindow.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/ITextureAsset.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/ElementComponents/UILayerTransformComponent.h>
#include <UI/Elements/UIImage.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIScrollArea.h>
#include <UI/Elements/UIStack.h>
#include <UI/Elements/UILabel.h>
#include <UI/Elements/UIButton.h>
#include <UI/IUICanvas.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/Elements/UIDocking.h>
#include <UI/Elements/UIDockingManager.h>
#include <UI/ElementComponents/UIResizeComponent.h>
#include <UI/Elements/UIDiv.h>
#include <Graphics/UIPacket.h>
#include <Graphics/UIDrawPacket.h>

DJINN_NS2(UI);

UIWindow::UIWindow(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIWindow::StaticInitialize() {
    Data::IAssetHost* host;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&host), Error, "Expected asset worldHost to be installed");

    // Get texture assets
    auto textureControl = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/ControlBarMB.png"_path);
    auto textureBackground = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/WindowMB.png"_path);
    auto textureClose = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/Close.png"_path);
    auto textureMax = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/Maximize.png"_path);
    auto textureMin = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/Minimize.png"_path);

    auto outerTransform = CreateComponent<UILayerTransformComponent>();

    {
        auto input = CreateComponent<UIInputComponent>();

        input->propagated.Add(this, [=](const Game::InputEvent &event, IUIElement* element, UIInputActionBuffer *actions) {
            auto transform = GetTransform();

            // Only push to foreground if valid event
            if (!event.IsFocusEvent()) {
                return;
            }

            if (auto dm = GetDockingManager()) {
                if (dm->IsFloating(transform)) {
                    dm->SetForeground(transform);
                }
            }
        });
    }

    {
        UIImage *body;
        CheckVoid(GetSystem()->CreateActor(GetScene(), &body));

        body->SetTexture(textureBackground);

        body->SetMarginGeometry({4, 4});

        auto transform = body->GetTransform();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        transform->SetTraceFallthrough(true);
        outerTransform->AddChild(transform);
    }

    UIStack *outerStack;
    CheckVoid(GetSystem()->CreateActor(GetScene(), &outerStack));

    outerStack->GetTransform()->SetScaling(UIScaling::eFill, UIScaling::eFill);
    outerStack->SetMode(UIStackMode::eVertical);
    outerStack->GetTransform()->SetTraceFallthrough(true);

    {
        UIImage *controlBar;
        CheckVoid(GetSystem()->CreateActor(GetScene(), &controlBar));

        controlBar->SetTexture(textureControl);

        auto controlTransform = controlBar->CreateComponent<UILayoutTransformComponent>();
        controlTransform->SetHorizontalScaling(UIScaling::eFill);
        controlTransform->SetDesiredSize({0, 20});

        controlBar->SetTransform(controlTransform);
        outerStack->AddChild(controlTransform);

        UIStack *titleStack;
        {
            CheckVoid(GetSystem()->CreateActor(GetScene(), &titleStack));

            auto stackTransform = titleStack->GetTransform();
            stackTransform->SetHorizontalAlignment(UIHorizontalAlignment::eLeft);
            stackTransform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
            stackTransform->SetTraceFallthrough(true);
            stackTransform->SetMargin({5, 0});
            controlTransform->AddChild(stackTransform);

            {
                UIImage *icon;
                CheckVoid(GetSystem()->CreateActor(GetScene(), &icon));

                auto transform = icon->GetTransform();
                transform->SetDesiredSize({10, 10});
                transform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
                titleStack->AddChild(transform);
            }

            {
                CheckVoid(GetSystem()->CreateActor(GetScene(), &title));

                title->SetText("Window");

                auto transform = title->GetTransform();
                transform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
                transform->SetDesiredSize({15, 15});
                transform->SetTraceFallthrough(true);
                titleStack->AddChild(transform);
            }
        }

        UIStack *buttonStack;
        {
            CheckVoid(GetSystem()->CreateActor(GetScene(), &buttonStack));

            auto stackTransform = buttonStack->GetTransform();
            stackTransform->SetHorizontalAlignment(UIHorizontalAlignment::eRight);
            stackTransform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
            stackTransform->SetTraceFallthrough(true);
            controlTransform->AddChild(stackTransform);

            {
                UIButton *minimize;
                CheckVoid(GetSystem()->CreateActor(GetScene(), &minimize));

                minimize->SetText("");
                minimize->SetTexture(textureMin);
                minimize->SetBoxGeometry();

                auto transform = minimize->GetTransform();
                transform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
                transform->SetDesiredSize({15, 15});
                buttonStack->AddChild(transform);
            }

            {
                UIButton *maximize;
                CheckVoid(GetSystem()->CreateActor(GetScene(), &maximize));

                maximize->SetText("");
                maximize->SetTexture(textureMax);
                maximize->SetBoxGeometry();

                maximize->GetFirstComponent<UIInputComponent>()->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed& event, UIInputActionBuffer* buffer) {
                    if (maximized) {
                        Minimize();
                    } else {
                        Maximize();
                    }
                });

                auto transform = maximize->GetTransform();
                transform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
                transform->SetDesiredSize({15, 15});
                buttonStack->AddChild(transform);
            }

            {
                UIButton *close;
                CheckVoid(GetSystem()->CreateActor(GetScene(), &close));

                close->SetText("");
                close->SetTexture(textureClose);
                close->SetBoxGeometry();

                auto transform = close->GetTransform();
                transform->SetVerticalAlignment(UIVerticalAlignment::eCenter);
                transform->SetDesiredSize({15, 15});
                buttonStack->AddChild(transform);
            }
        }

        auto input = controlBar->CreateComponent<UIInputComponent>();

        input->BindDragged(this, Bindff(OnDragged));
    }

    {
        UIDiv* div;
        CheckVoid(GetSystem()->CreateActor(GetScene(), &div));

        socket = COM::CastChecked<UILayoutTransformComponent>(div->GetTransform());
        socket->SetPadding({5, 5, 5, 5});
        socket->SetScaling(UIScaling::eFill, UIScaling::eFill);
        socket->SetTraceFallthrough(true);

        outerStack->AddChild(socket);
    }

    outerTransform->AddChild(outerStack->GetTransform());

    SetTransform(outerTransform);

    auto resize = CreateComponent<UIResizeComponent>();
    resize->SetFaces(UIResizeFace::eLeft | UIResizeFace::eBottomLeft | UIResizeFace::eBottom | UIResizeFace::eBottomRight | UIResizeFace::eRight);
}

void UIWindow::SetTitle(const Core::StringView &value) {
    title->SetText(value);
}

void UIWindow::SetScale(bool scale) {
    auto scaling = scale ? UIScaling::eFill : UIScaling::eNone;
    GetTransform()->SetScaling(scaling, scaling);
}

void UIWindow::AddChild(IUITransformComponent *child) {
    socket->AddChild(child);
}

IUILayer *UIWindow::GetLayer() {
    return static_cast<UILayerTransformComponent*>(GetTransform())->GetLayer();
}

void UIWindow::Maximize() {
    auto transform = GetTransform();

    if (!maximized) {
        minimizedSize = transform->GetDesiredSize();
        transform->SetScaling(UIScaling::eFill, UIScaling::eFill);
        transform->SetMargin({0, 0});
    }

    maximized = true;
}

void UIWindow::Minimize() {
    auto transform = GetTransform();

    if (maximized) {
        transform->SetScaling(UIScaling::eNone, UIScaling::eNone);
        transform->SetDesiredSize(minimizedSize);
    }

    maximized = false;
}

void UIWindow::OnDragged(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions) {
    auto transform = GetTransform();

    // Always minimize when dragged
    if (maximized) {
        transform->SetMargin({(event.position.x - transform->GetAbsolutePosition().x) * .5f, 0});
        Minimize();
    }

    // Docking parent?
    if (auto docking = GetDocking()) {
        auto dm = docking->GetDockingManager();

        // Remove this window from docker
        docking->RemoveChild(GetTransform());

        // No implicit scaling, maintain current shape
        transform->SetScaling(UIScaling::eNone, UIScaling::eNone);

        // Get current position for retranslation
        auto absolutePosition = transform->GetAbsolutePosition();

        dm->AddChild(transform);
        dm->SetFloating(transform);
        dm->SetForeground(transform);

        // ...
        transform->SetMargin(absolutePosition - dm->GetTransform()->GetAbsolutePosition());
    }

    // Are we part of a manager?
    if (auto dm = GetDockingManager()) {
        dm->ShowFitter(this, transform);
    }

    transform->SetMargin(transform->GetMargin() + event.delta);
    transform->MarkAsDirty(true);
}

Core::ArrayView<IDockable *> UIWindow::GetDockables() {
    return {};
}

IUIElement *UIWindow::GetDockingElement() {
    return this;
}

EDockingSlotSet UIWindow::GetDockingSlots() {
    return EDockingSlot::eNone;
}

void UIWindow::AddDockable(EDockingSlot slot, IDockable *dockable) {

}
