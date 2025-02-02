//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Math/Vector2I.h>
#include <COM/IUnknown.h>

namespace Djinn::Game {
    union InputEvent;

    /// Common results
    DJINN_COM_RESULT(InputDeviceDuplicate, true, "Game");
    DJINN_COM_RESULT(InputDeviceNotFound, true, "Game");

    class IInputDevice : public COM::TUnknown<IInputDevice> {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IInputDevice(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Pool all events from this device
        /// \param count
        /// \param out
        /// \return
        virtual COM::Result PoolEvents(UInt32& count, InputEvent* out) = 0;
    };
}