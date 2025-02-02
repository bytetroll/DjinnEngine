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
 on 3/15/2018.
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
