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

#include <UI/Elements/UIStack.h>
#include <UI/Elements/UIStackSeperator.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <Game/IActorSystem.h>

DJINN_NS2(UI);

UIStack::UIStack(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UIStack::StaticInitialize() {
    auto layout = CreateComponent<UIStackTransformComponent>();
    layout->SetScaling(UIScaling::eContent, UIScaling::eContent);

    SetTransform(layout);
}

void UIStack::AddChild(IUITransformComponent *child) {
    auto stack = static_cast<UIStackTransformComponent *>(GetTransform());

    ChildData data;
    data.child = child->GetWeakPtr();

    if (resizable && !childData.Empty()) {
        StackSeperator *seperator;
        CheckVoid(GetSystem()->CreateActor(GetScene(), &seperator));
        {
            seperator->SetMode(stack->GetMode());
            seperator->SetElements(childData.Last().child->GetElement(), child->GetElement());
        }

        stack->AddChild(seperator->GetTransform());

        data.leftSeperator = seperator->GetWeakPtr();
    }

    stack->AddChild(child);

    childData.Add(data);
}

void UIStack::RemoveChild(IUITransformComponent *child) {
    auto stack = static_cast<UIStackTransformComponent *>(GetTransform());

    USize index;
    if (childData.Any([&](auto& v) { return v.child == child; }, index)) {
        auto data = childData[index];

        if (data.leftSeperator) {
            stack->RemoveChild(data.leftSeperator->GetTransform());
            data.leftSeperator->Destroy();
        }

        USize next = index + 1;
        if (next < childData.Size()) {
            switch (stack->GetMode()) {
                case UIStackMode::eHorizontal: {
                    if (data.child->GetHorizontalScaling() == UIScaling::eFill) {
                        childData[next].child->SetHorizontalScaling(UIScaling::eFill);
                    }
                    if (childData[next].child->GetHorizontalScaling() == UIScaling::eFill) {
                        data.child->SetHorizontalScaling(UIScaling::eFill);
                    }
                    break;
                }
                case UIStackMode::eVertical: {
                    if (data.child->GetVerticalScaling() == UIScaling::eFill) {
                        childData[next].child->SetVerticalScaling(UIScaling::eFill);
                    }
                    if (childData[next].child->GetVerticalScaling() == UIScaling::eFill) {
                        data.child->SetVerticalScaling(UIScaling::eFill);
                    }
                    break;
                }
                case UIStackMode::eScanGrid: {
                    break;
                }
            }

            if (index != 0) {
                childData[next].leftSeperator->SetElements(childData[index - 1].child->GetElement(), childData[next].child->GetElement());
            }
        }

        stack->RemoveChild(data.child);

        childData.Remove(index);

        if (childData.Size() == 1) {
            switch (stack->GetMode()) {
                case UIStackMode::eHorizontal: {
                    if (data.child->GetHorizontalScaling() == UIScaling::eFill) {
                        childData[0].child->SetHorizontalScaling(UIScaling::eFill);
                    }
                    break;
                }
                case UIStackMode::eVertical: {
                    if (data.child->GetVerticalScaling() == UIScaling::eFill) {
                        childData[0].child->SetVerticalScaling(UIScaling::eFill);
                    }
                    break;
                }
                case UIStackMode::eScanGrid: {
                    break;
                }
            }
        }

    }
}

void UIStack::RemoveChilden() {
    for (auto&& child : childData) {
        if (child.child) {
            child.child->GetActor()->Destroy();
        }
        if (child.leftSeperator) {
            child.leftSeperator->Destroy();
        }
    }

    childData.Clear();
}

void UIStack::SetResizable(bool state) {
    this->resizable = state;
}
