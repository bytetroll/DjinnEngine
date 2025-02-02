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
 on 1/12/2018.
//

#include <Game/InputSystem.h>
#include <Game/IInputDevice.h>
#include <Game/InputConnection.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Base/IWindowSystem.h>
#include <Platform/Window.h>

DJINN_NS2(Game);

InputSystem::InputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IInputSystem(registry, outer, host) {

}

COM::Result InputSystem::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    return COM::kOK;
}

COM::Result InputSystem::Run(StepMode mode, double) {
    // Clear previous events
    stagedEvents.Clear();

    // Pool events
    UInt32 totalEventCount = 0;
    for (auto device : devices) {
        // Get count
        UInt32 eventCount;
        Check(device->PoolEvents(eventCount, nullptr));

        // Preallocate
        stagedEvents.Resize(totalEventCount + eventCount);

        // Stage
        if (eventCount != 0) {
            Check(device->PoolEvents(eventCount, &stagedEvents[totalEventCount]));
        }

        // ...
        totalEventCount += eventCount;
    }

    // Flush previously pressed
    for (auto kv : keyCodeLUT) {
        if (kv.second == InputState::ePressed) {
            keyCodeLUT[kv.first] = InputState::eDown;
        }
    }
    for (auto kv : virtualKeyCodeLUT) {
        if (kv.second == InputState::ePressed) {
            virtualKeyCodeLUT[kv.first] = InputState::eDown;
        }
    }

    // Process each event
    for (auto &event : stagedEvents) {
        switch (event.type) {
            default:
                break;
            case InputEventType::eKeyPressed:
                if (event.keyPressed.key.virtualKeyCode == Platform::VirtualKeyCode::eCharacter) {
                    keyCodeLUT[event.keyPressed.key.keyCode] = InputState::ePressed;
                } else {
                    virtualKeyCodeLUT[event.keyPressed.key.virtualKeyCode] = InputState::ePressed;
                }
                break;
            case InputEventType::eKeyReleased:
                if (event.keyReleased.key.virtualKeyCode == Platform::VirtualKeyCode::eCharacter) {
                    keyCodeLUT[event.keyReleased.key.keyCode] = InputState::eUp;
                } else {
                    virtualKeyCodeLUT[event.keyReleased.key.virtualKeyCode] = InputState::eUp;
                }
                break;
            case InputEventType::eKeyDown:
                if (event.keyDown.key.virtualKeyCode == Platform::VirtualKeyCode::eCharacter) {
                    keyCodeLUT[event.keyDown.key.keyCode] = InputState::eDown;
                } else {
                    virtualKeyCodeLUT[event.keyDown.key.virtualKeyCode] = InputState::eDown;
                }
                break;
            case InputEventType::eMousePressed:
                virtualKeyCodeLUT[event.mousePressed.virtualKeyCode] = InputState::ePressed;
                break;
            case InputEventType::eMouseReleased:
                virtualKeyCodeLUT[event.mousePressed.virtualKeyCode] = InputState::eUp;
                break;
            case InputEventType::eCharacter: {
                for (auto &connection : characterConnections) {
                    InputEvent::Character character;
                    character.virtualKeyCode = event.character.virtualKeyCode;
                    character.key = event.character.key;
                    connection.delegate.Invoke(character);
                    break;
                }
            }
            case InputEventType::eExit:
                break;
        }
    }

    // Process mappings
    mappingsCache.Clear(false);
    for (auto &mapping : mappings) {
        auto &state = (mapping.first.key.virtualKeyCode != Platform::VirtualKeyCode::eCharacter) ? virtualKeyCodeLUT[mapping.first.key.virtualKeyCode] : keyCodeLUT[mapping.first.key.keyCode];
        if (mapping.first.modifiers.All([&](auto keyCode) { return virtualKeyCodeLUT[keyCode] != InputState::eUp; })) {
            mappingsCache.Add({mapping.second, state});
        }
    }

    // Process axes
    axesCache.Clear(false);
    for (auto &axis : axes) {
        auto &state = (axis.first.key.virtualKeyCode != Platform::VirtualKeyCode::eCharacter) ? virtualKeyCodeLUT[axis.first.key.virtualKeyCode] : keyCodeLUT[axis.first.key.keyCode];
        axesCache.Add({axis.second, state});
    }

    // Lookup mapping actions
    for (auto &event : mappingConnections) {
        USize index;
        if (mappingsCache.Any([&](auto &v) { return v.first == event.mappingID && v.second == event.state; }, index)) {
            event.delegate.Invoke();
        }
    }

    // Lookup axes actions
    for (auto &event : axisConnections) {
        USize index;
        if (axesCache.Any([&](auto &v) { return v.first == event.axisID && v.second == event.state; }, index)) {
            event.delegate.Invoke(1.0f);
        }
    }

    // Lookup event actions
    for (auto &event : eventConnections) {
        if (event.type == InputEventType::eKeyDown) {
            for (const auto &kv : keyCodeLUT) {
                if (kv.second == InputState::eDown) {
                    InputEvent _event;
                    _event.keyDown = {};
                    _event.keyDown.key = kv.first;
                    event.delegate.Invoke(_event);
                }
            }
            for (const auto &kv : virtualKeyCodeLUT) {
                if (kv.second == InputState::eDown) {
                    InputEvent _event;
                    _event.keyDown = {};
                    _event.keyDown.key = kv.first;
                    event.delegate.Invoke(_event);
                }
            }
        } else {
            for (auto &stagedEvent : stagedEvents) {
                if (stagedEvent.type == event.type) {
                    event.delegate.Invoke(stagedEvent);
                }
            }
        }
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::Register(IInputDevice *device) {
    if (devices.Contains(device)) {
        return kInputDeviceDuplicate;
    }
    devices.Add(device);

    // OK
    return COM::kOK;
}

COM::Result InputSystem::Deregister(IInputDevice *device) {
    return devices.RemoveValue(device) ? COM::kOK : kInputDeviceNotFound;
}

StepModeSet InputSystem::GetStepMode() {
    return StepMode::eOnce;
}

COM::Result InputSystem::AddMapping(const MappingID &id, const InputMapping &event) {
    USize existing;
    if (mappings.Any([&](auto v) { return v.second == id; }, existing)) {
        mappings[existing].first = event;
    } else {
        mappings.Add({event, id});
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::RemoveMapping(const MappingID &id) {
    mappings.RemoveOnce([&](auto v) { return v.second == id; });

    // OK
    return COM::kOK;
}

COM::Result InputSystem::AddAxis(const AxisID &id, const InputAxis &event) {
    USize existing;
    if (axes.Any([&](auto v) { return v.second == id; }, existing)) {
        axes[existing].first = event;
    } else {
        axes.Add({event, id});
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::RemoveAxis(const AxisID &id) {
    axes.RemoveOnce([&](auto v) { return v.second == id; });

    // OK
    return COM::kOK;
}

COM::Result InputSystem::BindCharacter(const InputCharacterDelegate &delegate, InputConnection *connection) {
    auto conid = characterConnections.Size();
    characterConnections.Add({conid, delegate});

    // Setup connection
    if (connection) {
        connection->Assign([&] {
            characterConnections.RemoveOnce([&](auto &v) { return v.id == conid; });
        });
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::BindMapping(const MappingID &id, InputState state, const InputMappingDelegate &delegate, InputConnection *connection) {
    auto conid = mappingConnections.Size();
    mappingConnections.Add(MappingConnection{conid, id, state, delegate});

    // Setup connection
    if (connection) {
        connection->Assign([&] {
            mappingConnections.RemoveOnce([&](auto &v) { return v.id == conid; });
        });
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::BindAxis(const AxisID &id, InputState state, const InputAxisDelegate &delegate, InputConnection *connection) {
    auto conid = axisConnections.Size();
    axisConnections.Add(AxisConnection{conid, id, state, delegate});

    // Setup connection
    if (connection) {
        connection->Assign([&] {
            axisConnections.RemoveOnce([&](auto &v) { return v.id == conid; });
        });
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::BindEvent(const InputEventType &type, const InputEventDelegate &delegate, InputConnection *connection) {
    auto conid = eventConnections.Size();
    eventConnections.Add(EventConnection{conid, type, delegate});

    // Setup connection
    if (connection) {
        connection->Assign([&] {
            eventConnections.RemoveOnce([&](auto &v) { return v.id == conid; });
        });
    }

    // OK
    return COM::kOK;
}

COM::Result InputSystem::Configure(Base::PipelineBuilder& builder) {
    return COM::kOK;
}

COM::Result InputSystem::SetMousePosition(const Math::Vector2 &position) {
    // TODO: This does not belong here and is a temporary solution
    Base::IWindowSystem *system;
    if (!registry->GetPipeline()->GetInterface(&system)) {
        return COM::kError;
    }

    system->GetWindow()->SetLocalMousePosition(Math::Vector2I(position.x, position.y));
    return COM::kOK;
}
