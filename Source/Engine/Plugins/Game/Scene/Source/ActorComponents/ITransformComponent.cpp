//
// Created by Dev on 8/30/2018.
//

#include <Game/ActorComponents/ITransformComponent.h>
#include <Game/IActor.h>

DJINN_NS2(Game);

void ITransformComponent::UpdateTransform(bool force) {
    if (isDirty || force) {
        Update();

        for (auto &&child : children) {
            child->UpdateTransform(true);
        }
    } else {
        if (isChildDirty) {
            for (auto &&child : dirtyChildren) {
                child->UpdateTransform();
            }
        }
    }

    // ...
    dirtyChildren.Clear();
    isDirty = false;
    isAdded = false;
}

void ITransformComponent::Clean() {
    for (auto &&child : dirtyChildren) {
        child->Clean();
    }
    dirtyChildren.Clear();
    MarkAsClean();
}

void ITransformComponent::SetParent(ITransformComponent *_new) {
    if (!_new) {
        if (parent) {
            parent->children.RemoveValue(GetWeakPtr());
        }

        parent = nullptr;
    } else {
        GetActor()->AssertReference(_new->GetActor());

        if (parent) {
            parent->children.RemoveValue(GetWeakPtr());
        }

        this->parent = _new->GetWeakPtr();
        _new->children.Add(GetWeakPtr());
    }


    MarkAsDirty(true);
}
