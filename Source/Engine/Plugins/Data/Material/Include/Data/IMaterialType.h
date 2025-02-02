//
// Created by Dev on 4/6/2018.
//

#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>

namespace Djinn::Data {
    class IMaterialType : public COM::TUnknown<IMaterialType> {
    public:
        DJINN_COM_INTERFACE();

        IMaterialType(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get shader file path
        /// \return
        virtual Core::String GetHeaderPath() = 0;

        /// Get shader file path
        /// \return
        virtual Core::String GetKernelPath() = 0;
    };
}