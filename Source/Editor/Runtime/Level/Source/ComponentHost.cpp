//
// Created by Dev on 7/15/2018.
//

#include <Editor/ComponentHost.h>
#include <Editor/Components/RuntimeComponent.h>
#include <Game/IActor.h>

DJINN_NS2(Editor);

COM::Result ComponentHost::AddAttribute(const COM::ClassID &cid, const ComponentAttributeSet &attributes) {
    auto &com = componentMap[cid];

    // Add attributes
    com.attributes += attributes;

    // OK
    return COM::kOK;
}

COM::Result ComponentHost::Create(Game::IActor *actor, const COM::ClassID &cid, Game::IActorComponent **out) {
    auto &com = componentMap[cid];

    // Run in editor?
    if (com.attributes & ComponentAttribute::eRunInEditor) {
        *out = actor->CreateComponent(cid);
    } else {
        // Non editor components are always runtime components
        auto runtime = actor->CreateComponent<RuntimeComponent>();
        runtime->SetComponentID(cid);

        // ...
        *out = runtime;
    }

    // OK
    return COM::kOK;
}
