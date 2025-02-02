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

#include <UI/Elements/UITable.h>
#include <UI/ElementComponents/UITableTransformComponent.h>
#include <Game/IActorSystem.h>

DJINN_NS2(UI);

UITable::UITable(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UITable::StaticInitialize() {
    auto table = CreateComponent<UITableTransformComponent>();
    table->SetScaling(UIScaling::eContent, UIScaling::eContent);

    SetTransform(table);
}

void UITable::SetCell(UInt32 row, UInt32 column, IUITransformComponent *child) {
    auto table = static_cast<UITableTransformComponent *>(GetTransform());

    table->SetCell(row, column, child->GetElement());
}

void UITable::RemoveCell(UInt32 row, UInt32 column) {
    auto table = static_cast<UITableTransformComponent *>(GetTransform());

    table->SetCell(row, column, nullptr);
}

void UITable::SetResizable(bool state) {
    this->resizable = state;
}

void UITable::Configure(UInt32 rows, UInt32 columns) {
    static_cast<UITableTransformComponent*>(GetTransform())->Configure(rows, columns);
}
