//
// Created by Dev on 3/15/2018.
//

#include <Game/Entity.h>
#include <Game/IEntitySystem.h>
#include <Game/IEntityComponent.h>
#include <Game/IEntityComponentHost.h>

DJINN_NS2(Game);

OpaqueEntityComponent Entity::AddComponent(const COM::ClassID &cid) const {
    // Create component
    EntityComponentReference componentReference;
    if (!ref.system->CreateComponent(ref, cid, &componentReference)) {
        DJINN_THROW EComponentCreationFailed();
    }

    // Wrap
    return OpaqueEntityComponent(componentReference, ref.system->GetCommitHandle(), cid);
}

OpaqueEntityComponent Entity::GetComponent(const COM::ClassID &cid) const {
    // Get component
    IEntityComponent *component;
    if (!ref.system->GetComponentSystem(cid, &component)) {
        return nullptr;
    }

    // Get worldHost
    auto host = component->GetHost();

    // Get from entity id
    IEntityComponentState* dummy;
    if (!host->GetComponentPtr(ref.id, &dummy)) {
        return nullptr;
    }

    // ...
    return OpaqueEntityComponent(EntityComponentReference(ref, component, host), ref.system->GetCommitHandle(), cid);
}

bool Entity::Destroy() const {
    return ref.system->DestroyEntity(ref) != COM::kError;
}
