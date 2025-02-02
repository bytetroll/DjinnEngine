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

#include <UI/Elements/UIScrollArea.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <Data/IAssetHost.h>
#include <Base/DefaultLogs.h>
#include <UI/Elements/UIButton.h>
#include <UI/Elements/UIDiv.h>
#include <UI/ElementComponents/UIDragDropComponent.h>

DJINN_NS2(UI);

UIScrollArea::UIScrollArea(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIScrollArea::StaticInitialize() {
    auto layout = CreateComponent<UIStackTransformComponent>();
    layout->SetMode(UIStackMode::eHorizontal);

    // Content
    {
        if (!GetSystem()->CreateActor(GetScene(), &contentArea)) {
            return;
        }

        auto contentTransform = contentArea->GetTransform();
        contentTransform->SetScaling(UIScaling::eFill, UIScaling::eNone);
        contentTransform->SetDesiredSize({0, 1e6});

        // Update the scrollbar when the content has changed
        contentTransform->updated.Add(this, Bindff(ContentUpdated));

        layout->AddChild(contentTransform);
    }

    // Scroll bar
    {
        CheckVoid(GetSystem()->CreateActor(GetScene(), &scrollbar));

        scrollbar->SetText("");

        auto btnTransform = scrollbar->GetTransform();
        btnTransform->SetDesiredSize({15, 100});
        btnTransform->SetMargin({3.5f, 0});

        auto dd = scrollbar->CreateComponent<UIDragDropComponent>();
        dd->SetMask({0, 1});
        dd->SetRestricted(true);

        layout->AddChild(btnTransform);

        dd->movedEvents.Add(this, Bindff(ScrollbarUpdated));
    }

    SetTransform(layout);
}

void UIScrollArea::ScrollbarUpdated() {
    if (!content)
        return;

    auto contentTransform = COM::Cast<UILayoutTransformComponent>(contentArea->GetTransform());

    auto buttonTransform = scrollbar->GetTransform();

    /**
     * ----------
     *     |
     *     |
     *     |
     *    ---  -|
     *    | |   |
     *    | |   | H
     *    | |   |
     *    ---  -|    -|
     *     |          | R
     * ---------  M  -|
     */

    float maxOffset = GetTransform()->GetDesiredSize().y - buttonTransform->GetDesiredSize().y;

    float offset = buttonTransform->GetMargin().y;

    float scrollAmount = offset / maxOffset;

    float margin = -Math::Max(0, content->GetMaximumDesiredSize().y - GetTransform()->GetDesiredSize().y) * scrollAmount;

    contentTransform->SetPadding({0, margin, 0, 0});
}

void UIScrollArea::SetContent(IUITransformComponent *child) {
    contentArea->AddChild(child);
    content = child->GetWeakPtr();
}

void UIScrollArea::ContentUpdated() {
    if (!content)
        return;

    auto buttonTransform = scrollbar->GetTransform();

    float value = GetTransform()->GetDesiredSize().y / content->GetMaximumDesiredSize().y;

    if (value >= 1.0f) {
        buttonTransform->SetHidden(true);
    } else {
        buttonTransform->SetHidden(false);
        buttonTransform->SetDesiredSize({15, Math::Max(value * GetTransform()->GetDesiredSize().y, 15)});
    }
}
