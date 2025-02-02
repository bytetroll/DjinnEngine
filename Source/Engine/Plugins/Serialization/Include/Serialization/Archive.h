//
// Created by Dev on 10/18/2017.
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