#pragma once

#include <COM/IUnknown.h>
#include <Async/Async.h>
#include "PipelineBuilder.h"

namespace Djinn::Base {
    /// Common results
    DJINN_COM_RESULT(SystemNotFound, true, "Base");
    DJINN_COM_RESULT(SystemDuplicate, true, "Base");

    class ISystem : public COM::TUnknown<ISystem> {
    public:
        DJINN_COM_INTERFACE();

        ISystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Configure
        /// \param pipeline
        /// \return
        virtual COM::Result Configure(PipelineBuilder& builder) = 0;
    };
}