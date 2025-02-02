//
// Created by Dev on 8/15/2018.
//

#include <Game/IActor.h>
#include <Game/IActorComponent.h>
#include <Game/IActorSystem.h>
#include <Host/IRegistry.h>

DJINN_NS2(Game);

IActor::IActor(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorSystemInstance(registry, outer, host) {

}

void IActor::Destroy() {
    Sink(GetSystem()->DestroyActor(this));
    isDeleted = true;
}

void IActor::Register(IActorComponent *component) {
    // Remove dangling references
    if (component->owner) {
        component->owner->components.RemoveValue(component->GetWeakPtr());
    }

    // Set owner
    component->owner = this;

    // Register
    components.Add(component->GetWeakPtr());

    // Set lifetime owner
    Bind(component);
}

IActorComponent *IActor::CreateComponent(const COM::ClassID &cid) {
    // Attempt to create component
    IActorComponent* out;
    if (!GetSystem()->CreateComponent(GetScene(), cid, this, &out)) {
        return nullptr;
    }

    // OK
    return out;
}

IActorComponent *IActor::GetFirstComponent(const COM::ClassID &cid) {
    for (auto&& com : components) {
        if (com->GetClassID() == cid) {
            return com.Get();
        }
    }
    return nullptr;
}

IActorComponent *IActor::GetFirstComponent(const COM::InterfaceID &iid) {
    for (auto&& com : components) {
        void* dummy;
        if (com->QueryInterface(iid, &dummy)) {
            return com.Get();
        }
    }
    return nullptr;
}

void IActor::RemoveComponent(IActorComponent *component) {

}

void IActor::SetName(const Core::StringView &value) {
    name = value.ToString();
}

COM::Result IActor::Released() {
    Destroyed();
    return COM::kOK;
}
