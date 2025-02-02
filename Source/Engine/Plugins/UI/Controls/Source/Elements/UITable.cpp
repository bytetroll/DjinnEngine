//
// Created by Dev on 8/23/2018.
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
