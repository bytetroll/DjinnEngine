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
 on 8/31/2018.
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

