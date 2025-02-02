//
// Created by Dev on 4/7/2018.
//
#pragma once

#include <Data/IMaterialLayout.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH BaseMaterialLayout : public IMaterialLayout {
    public:
        DJINN_COM_CLASS();

        BaseMaterialLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual Core::String GetPath() override;
    };
}