//
// Created by Dev on 1/12/2018.
//
#pragma once

#include <Platform/WindowEvent.h>
#include <Core/Array.h>
#include "IInputDevice.h"

namespace Djinn::Platform {
    class Window;
}

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_INPUT WindowInputDevice : public IInputDevice {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        WindowInputDevice(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this input device
        /// \param window
        /// \return
        COM::Result Initialize(Platform::Window* window);

        /// Overrides
                virtual COM::Result PoolEvents(UInt32 &count, InputEvent *out) override;

    private:
        Platform::Window* window;
        Core::Array<Platform::WindowEvent> stagedEvents;
    };
}