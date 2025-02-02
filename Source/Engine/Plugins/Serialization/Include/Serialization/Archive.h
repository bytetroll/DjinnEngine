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

#include <Core/IStream.h>
#include <Core/String.h>
#include <type_traits>
#include <Core/TypeCommon.h>
#include <COM/IUnknown.h>
#include <Core/BinaryStream.h>

namespace Djinn::Serialization {
    class ISerializerHost;

    /// Common results
    DJINN_COM_RESULT(TypeMismatch, true, "Serialization");

    class DJINN_EXPORT_PLUGINS_SERIALIZATION Archive : public Core::BinaryStream {
    public:
        Archive(ISerializerHost *host, Core::IStream *stream);

        /// Use binary stream operators
        using Core::BinaryStream::operator<<;
        using Core::BinaryStream::operator>>;

        /// IUnknown write
        /// \param com
        /// \return
        COM::Result operator<<(COM::IUnknown *com);

        /// IUnknown read
        /// \param com
        /// \return
        COM::Result operator>>(COM::IUnknown **com);

        /// IUnknown read (In place)
        /// \param com
        /// \return
        COM::Result operator>>(COM::IUnknown *com);

        /// IUnknown derived read
        template<typename T>
        COM::Result operator>>(T **com) {
            COM::IUnknown *obj;
            if (COM::CheckedResult result = (*this) >> &obj) {
                return result;
            }

            // Class or interface?
            if constexpr(COM::HasKCID<T>::kValue) {
                // Attempt to cast class
                if (!(*com = COM::Cast<T>(obj))) {
                    return kTypeMismatch;
                }
                return COM::kOK;
            } else {
                // Attempt to query interface
                return obj->QueryInterface(com);
            }
        }

        /// IUnknown derived read (In place)
        template<typename T>
        COM::Result operator>>(T *com) {
            return (*this) >> COM::Cast<COM::IUnknown>(com);
        }

        /// Allow for dummy COM::Result check on templated types
        /// \return
        operator bool() {
            return true;
        }

        /// Get parent worldHost
        /// \return
        ISerializerHost *GetHost() const {
            return host;
        }

    private:
        ISerializerHost *host;
    };
}