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
 on 8/15/2018.
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
