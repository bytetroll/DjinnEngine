//
// Created by Dev on 12/21/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::GHA {
    class IScheduler;
    class IInstance;
    class IDevice;
    class ISwapchain;
    class IAdapter;

    class IInterpreter : public COM::TUnknown<IInterpreter> {
    public:
        DJINN_COM_INTERFACE();

        IInterpreter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this interpreter
        /// \return
        virtual COM::Result Initialize(IInstance* instance, IAdapter* adapter, IDevice* device, ISwapchain* swapchain) = 0;

        /// Compile from a scheduler
        /// \param scheduler - Must be compatible
        /// \return
        virtual COM::Result Compile(IScheduler* scheduler) = 0;

        /// Get gpu delta time
        /// \return
        virtual double GetGPUDelta() = 0;
    };
}