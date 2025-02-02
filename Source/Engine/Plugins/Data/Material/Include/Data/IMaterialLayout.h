//
// Created by Dev on 4/6/2018.
//

#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>

namespace Djinn::Data {
    class IMaterialLayout : public COM::TUnknown<IMaterialLayout> {
    public:
        DJINN_COM_INTERFACE();

        IMaterialLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get shader file path
        /// \return
        virtual Core::String GetPath() = 0;
    };
}