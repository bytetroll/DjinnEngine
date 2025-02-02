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
 on 8/20/2018.
//
#pragma once

#include <Game/IActorComponent.h>
#include <Game/InputConnection.h>
#include <Core/FixedArray.h>
#include <Game/IInputSystem.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_INPUT InputComponent : public IActorComponent {
    public:
        DJINN_COM_CLASS();

        InputComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorComponent(registry, outer, host) {

        }

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Bind to a character event
        /// \param delegate
        /// \return
        void BindCharacter(const Game::InputCharacterDelegate &delegate);

        /// Bind to a mapping event
        /// \param id
        /// \param state
        /// \param delegate
        /// \return
        void BindMapping(const Game::MappingID &id, Game::InputState state, const Game::InputMappingDelegate &delegate);

        /// Bind to an axis event
        /// \param id
        /// \param state
        /// \param delegate
        /// \return
        void BindAxis(const Game::AxisID &id, Game::InputState state, const Game::InputAxisDelegate &delegate);

        /// Bind to an event
        /// \param id
        /// \param state
        /// \param delegate
        /// \return
        void BindEvent(const Game::InputEventType &type, const Game::InputEventDelegate &delegate);

        /// Unbind all events
        void Unbind();

    private:
        IInputSystem *system = nullptr;
        Core::FixedArray32<InputConnection> connections;
    };
}