//
// Created by Dev on 4/7/2018.
//
#pragma once

#include <Data/IMaterialType.h>

namespace Djinn::Graphics {
    class DJINN_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING DeferredGBufferMaterialType : public Data::IMaterialType {
    public:
        DJINN_COM_CLASS();

        DeferredGBufferMaterialType(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual Core::String GetHeaderPath() override;
        virtual Core::String GetKernelPath() override;
    };
}