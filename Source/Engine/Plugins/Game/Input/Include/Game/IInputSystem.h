//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Game/IStepSystem.h>
#include "InputID.h"
#include "InputMapping.h"
#include "InputAxis.h"
#include "InputEvent.h"
#include "InputState.h"
#include "InputConnection.h"

namespace Djinn::Game {
    class IInputDevice;

    /// Delegates
    using InputCharacterDelegate = Core::Delegate<void(const InputEvent::Character &)>;
    using InputMappingDelegate = Core::Delegate<void()>;
    using InputAxisDelegate = Core::Delegate<void(float value)>;
    using InputEventDelegate = Core::Delegate<void(const InputEvent &)>;

    class DJINN_EXPORT_PLUGINS_GAME_INPUT IInputSystem : public IStepSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IInputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IStepSystem(registry, outer, host) {

        }

        /// Register a device
        /// \param device
        /// \return
        virtual COM::Result Register(IInputDevice *device) = 0;

        /// Deregister a device
        /// \param device
        /// \return
        virtual COM::Result Deregister(IInputDevice *device) = 0;

        /// Bind to a character event
        /// \param delegate
        /// \param connection
        /// \return
        virtual COM::Result BindCharacter(const InputCharacterDelegate &delegate, InputConnection *connection) = 0;

        /// Bind to a mapping
        /// \param id
        /// \param delegate
        /// \param out
        /// \return
        virtual COM::Result BindMapping(const MappingID &id, InputState state, const InputMappingDelegate &delegate, InputConnection *connection) = 0;

        /// Bind to an axis
        /// \param id
        /// \param delegate
        /// \param out
        /// \return
        virtual COM::Result BindAxis(const AxisID &id, InputState state, const InputAxisDelegate &delegate, InputConnection *connection) = 0;

        /// Bind to an event
        /// \param type
        /// \param event
        /// \return
        virtual COM::Result BindEvent(const InputEventType &type, const InputEventDelegate &delegate, InputConnection *connection) = 0;

        /// Add an input mapping and bind it to id
        /// \param id
        /// \param event
        /// \return
        virtual COM::Result AddMapping(const MappingID &id, const InputMapping &event) = 0;

        /// Remove an input mapping by id
        /// \param id
        /// \return
        virtual COM::Result RemoveMapping(const MappingID &id) = 0;

        /// Add an input axis and bind it to id
        /// \param id
        /// \param event
        /// \return
        virtual COM::Result AddAxis(const AxisID &id, const InputAxis &event) = 0;

        /// Remove an input axis by id
        /// \param id
        /// \return
        virtual COM::Result RemoveAxis(const AxisID &id) = 0;

        /// Set the mouse position
        /// \param position
        /// \return
        virtual COM::Result SetMousePosition(const Math::Vector2& position) = 0;
    };
}