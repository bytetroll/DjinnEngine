//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Core/HashMap.h>
#include <Core/SlotArray.h>
#include <Core/HashSet.h>
#include <Core/Buffer.h>
#include "IInputSystem.h"
#include "InputEvent.h"
#include "InputState.h"

namespace Djinn::Game {
    class IInputDevice;

    class DJINN_EXPORT_PLUGINS_GAME_INPUT InputSystem : public IInputSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        InputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result Run(StepMode mode, double delta) override;
        COM::Result Register(IInputDevice *device) override;
        COM::Result Deregister(IInputDevice *device) override;
        StepModeSet GetStepMode() override;
        COM::Result AddMapping(const MappingID &id, const InputMapping &event) override;
        COM::Result RemoveMapping(const MappingID &id) override;
        COM::Result AddAxis(const AxisID &id, const InputAxis &event) override;
        COM::Result RemoveAxis(const AxisID &id) override;
        COM::Result BindCharacter(const InputCharacterDelegate &delegate, InputConnection *connection) override;
        COM::Result BindMapping(const MappingID &id, InputState state, const InputMappingDelegate &delegate, InputConnection *connection) override;
        COM::Result BindAxis(const AxisID &id, InputState state, const InputAxisDelegate &delegate, InputConnection *connection) override;
        COM::Result BindEvent(const InputEventType &type, const InputEventDelegate &delegate, InputConnection *connection) override;
        COM::Result Configure(Base::PipelineBuilder& builder) override;
        COM::Result SetMousePosition(const Math::Vector2 &position) override;

    public:
        struct CharacterConnection {
            USize id;
            InputCharacterDelegate delegate;
        };

        struct MappingConnection {
            USize id;
            MappingID mappingID;
            InputState state;
            InputMappingDelegate delegate;
        };

        struct AxisConnection {
            USize id;
            AxisID axisID;
            InputState state;
            InputAxisDelegate delegate;
        };

        struct EventConnection {
            USize id;
            InputEventType type;
            InputEventDelegate delegate;
        };

        Core::Array<IInputDevice *> devices;
        Core::Buffer<InputEvent> stagedEvents;
        Core::SlotArray<Core::Pair<InputMapping, MappingID>> mappings;
        Core::SlotArray<Core::Pair<InputAxis, AxisID>> axes;
        Core::SlotArray<CharacterConnection> characterConnections;
        Core::SlotArray<MappingConnection> mappingConnections;
        Core::SlotArray<AxisConnection> axisConnections;
        Core::SlotArray<EventConnection> eventConnections;

        Core::HashMap<Platform::KeyCode, InputState> keyCodeLUT;
        Core::HashMap<Platform::VirtualKeyCode, InputState> virtualKeyCodeLUT;

        Core::Array<Core::Pair<MappingID, InputState>> mappingsCache;
        Core::Array<Core::Pair<AxisID, InputState>> axesCache;
    };
}