//
// Created by Dev on 3/15/2018.
//

#include <Game/Entity.h>
#include <Game/IEntitySystem.h>
#include <Game/IEntityComponentHost.h>
#include <Game/EntityComponentHost.h>
#include <Game/SortedEntityComponentHost.h>
#include <Game/IEntityComponentState.h>

DJINN_NS2(Game);

IEntityComponentState *Detail::GetEntityComponentPtr(const EntityComponentReference &ref) {
    IEntityComponentState *ptr;
    if (!ref.host->GetComponentPtr(ref.entity.id, &ptr)) {
        return nullptr;
    }
    return ptr;
}

bool Detail::DestroyEntityComponent(const EntityComponentReference &ref) {
    return ref.entity.system->DestroyComponent(ref) != COM::kError;
}

