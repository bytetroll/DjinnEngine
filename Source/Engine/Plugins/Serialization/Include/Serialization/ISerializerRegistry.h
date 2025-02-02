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
 on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Serialization {
    class ISerializer;

    // Common results
    DJINN_COM_RESULT(NoSerializer, true, "Data");

    class ISerializerRegistry : public COM::TUnknown<ISerializerRegistry> {
    public:
        DJINN_COM_INTERFACE();

        ISerializerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add a serializer with no attachments
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(ISerializer *serializer) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(const COM::ClassID& id, ISerializer* serializer) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddSerializer(const COM::ClassID& id, const COM::ClassID& serializerID) = 0;

        /// Remove a serializer from id
        /// \param id
        /// \return
        virtual COM::Result RemoveSerializer(const COM::ClassID& id) = 0;

        /// Get serializer from id
        /// \param id
        /// \return
        virtual COM::Result GetSerializer(const COM::ClassID& id, ISerializer** out) = 0;

        /// Get serializer with id
        /// \param id
        /// \return
        virtual COM::Result GetSerializerWithID(const COM::ClassID& id, ISerializer** out) = 0;
    };
}