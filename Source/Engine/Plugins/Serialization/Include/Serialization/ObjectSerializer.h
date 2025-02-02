//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 10/19/2017.
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