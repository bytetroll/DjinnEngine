//
// Created by Dev on 10/19/2017.
//
#pragma once

#include <Reflection/IReflectionRegistry.h>
#include "ISerializer.h"

namespace Djinn::Serialization {
    class SerializerHost;

    class DJINN_EXPORT_PLUGINS_SERIALIZATION ObjectSerializer : public ISerializer {
    public:
        DJINN_COM_CLASS();

        ObjectSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Serialize(Archive &archive, COM::IUnknown *object) override;
        virtual COM::Result Deserialize(Archive &archive, COM::IUnknown **outInstance) override;
        virtual COM::Result Deserialize(Archive &archive, COM::IUnknown *object) override;

    private:
        /// Get number of classes
        /// \return
        USize GetClassCount(Reflection::IClassInfo* _class);

        /// Serialize all sub classes
        /// \param archive
        /// \param _class
        /// \param object
        /// \return
        COM::Result SerializeClasses(Archive& archive, Reflection::IClassInfo* _class, COM::IUnknown *object);

        /// Serialize class
        /// \return
        COM::Result SerializeClass(Archive &archive, Reflection::IClassInfo* _class, COM::IUnknown *object);

        /// Deserialize class
        /// \return
        COM::Result DeserializeClass(Archive &archive, COM::IUnknown *_class);

    private:
        SerializerHost* host = nullptr;
        Reflection::IReflectionRegistry* reflection = nullptr;
    };
}