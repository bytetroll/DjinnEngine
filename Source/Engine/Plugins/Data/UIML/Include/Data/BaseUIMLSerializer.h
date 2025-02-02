//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "IUIMLSerializer.h"
#include "UIMLContent.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_UIML BaseUIMLSerializer : public IUIMLSerializer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        BaseUIMLSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Serialize(Serialization::Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown **object) override;
        virtual COM::Result Deserialize(Serialization::Archive &archive, COM::IUnknown *object) override;

    private:
        COM::Result SerializeValue(Serialization::Archive &archive, const Data::UIML::StyleValue& value);
        COM::Result DeserializeValue(Serialization::Archive &archive, Data::UIML::StyleValue& value);
        COM::Result SerializeBody(Serialization::Archive &archive, const Data::UIML::CompiledBody& value);
        COM::Result DeserializeBody(Serialization::Archive &archive, Data::UIML::CompiledBody& value);
    };
}