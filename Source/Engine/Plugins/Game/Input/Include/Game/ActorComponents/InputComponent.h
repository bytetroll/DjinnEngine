//
// Created by Dev on 8/20/2018.
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