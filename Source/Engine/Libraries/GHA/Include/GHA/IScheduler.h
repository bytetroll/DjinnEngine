//
// Created by Dev on 12/21/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::GHA {
    union Command;
    class ScheduledNode;
    class ISwapchain;
    struct SchedulerContext;
    class IDevice;

    class IScheduler : public COM::TUnknown<IScheduler> {
    public:
        DJINN_COM_INTERFACE();

        IScheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this scheduler
        /// \return
        virtual COM::Result Initialize(IDevice* device, ISwapchain *swapchain) = 0;

        /// Get scheduler context
        /// \return
        virtual SchedulerContext* GetContext() = 0;

        /// Schedule a batch of nodes
        /// \param count
        /// \param out
        virtual COM::Result Schedule(UInt32 count, ScheduledNode** nodes) = 0;

        /// Compile all scheduled nodes, will flush current scheduled nodes
        /// \return
        virtual COM::Result Compile() = 0;
    };
}