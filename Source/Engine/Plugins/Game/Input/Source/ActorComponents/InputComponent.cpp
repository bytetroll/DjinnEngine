//
// Created by Dev on 8/31/2018.
//

#include <Game/ActorComponents/InputComponent.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Game);

void InputComponent::StaticInitialize() {
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&system), Error, "Expected input system to be installed");
}

void InputComponent::BindCharacter(const Game::InputCharacterDelegate &delegate) {
    connections.Add({});
    CheckVoid(system->BindCharacter(delegate, &connections.Last()));
    connections.Last().Manual();
}

void InputComponent::BindMapping(const Game::MappingID &id, Game::InputState state, const Game::InputMappingDelegate &delegate) {
    connections.Add({});
    CheckVoid(system->BindMapping(id, state, delegate, &connections.Last()));
    connections.Last().Manual();
}

void InputComponent::BindAxis(const Game::AxisID &id, Game::InputState state, const Game::InputAxisDelegate &delegate) {
    connections.Add({});
    CheckVoid(system->BindAxis(id, state, delegate, &connections.Last()));
    connections.Last().Manual();
}

void InputComponent::BindEvent(const Game::InputEventType &type, const Game::InputEventDelegate &delegate) {
    connections.Add({});
    CheckVoid(system->BindEvent(type, delegate, &connections.Last()));
    connections.Last().Manual();
}

void InputComponent::Unbind() {
    for (auto &bind : connections) {
        bind.Unbind();
    }
    connections.Clear();
}

