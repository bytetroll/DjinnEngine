//
// Created by Dev on 1/28/2018.
//
#pragma once


#include "IFontSerializer.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_FONT BaseFontSerializer : public IFontSerializer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        BaseFontSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Serialize(Serialization::Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown **object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown *object) override;
    };
}