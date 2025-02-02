#pragma once

#include <Core/Unique.h>
#include <Core/Types.h>
#include <Core/CompileHash.h>
#include <Core/Template/MemberType.h>
#include <Core/TypeName.h>
#include <type_traits>

namespace Djinn {
    namespace COM {
        /// ID Aliases
        DJINN_UNIQUEID(ClassID, UInt64);
        DJINN_UNIQUEID(InterfaceID, UInt64);
        DJINN_UNIQUEID(InstanceID, UInt64);

        /// ID Helpers
#define DJINN_COM_CLASSID() \
        struct _CIDT; \
        DJINN_STATICCONSTEXPR Djinn::COM::ClassID kCID = Djinn::COM::ClassID(GetSuperTypeNameCRC64<_CIDT>())
#define DJINN_COM_INTERFACEID() \
        struct _IIDT; \
        DJINN_STATICCONSTEXPR Djinn::COM::InterfaceID kIID = Djinn::COM::InterfaceID(GetSuperTypeNameCRC64<_IIDT>())
#define DJINN_COM_CLASSSTR(STR_NAME) \
        DJINN_STATICCONSTEXPR Djinn::COM::ClassID kCID = Djinn::COM::ClassID(STR_NAME##_crc64)
#define DJINN_COM_INTERFACESTR(STR_NAME) \
        DJINN_STATICCONSTEXPR Djinn::COM::InterfaceID kIID = Djinn::COM::InterfaceID(STR_NAME##_crc64)
#define DJINN_COM_INSTANCEID(STR_NAME) \
        DJINN_STATICCONSTEXPR Djinn::COM::InstanceID kIID = Djinn::COM::InstanceID(STR_NAME##_crc64)

        /// ID Helpers
        DJINN_SPAWNABLE_HASMEMBER(HasKCID, kCID);
        DJINN_SPAWNABLE_HASMEMBER(HasKIID, kIID);

        namespace Detail {
            /// Default fallback
            template <typename T>
            struct SafeClassID { DJINN_STATICCONSTEXPR Djinn::COM::ClassID kCID = T::kCID; };

            /// ...
            template<> struct SafeClassID<char> { DJINN_COM_CLASSSTR("char"); };
            template<> struct SafeClassID<Int8> { DJINN_COM_CLASSSTR("Int8"); };
            template<> struct SafeClassID<Int16> { DJINN_COM_CLASSSTR("Int16"); };
            template<> struct SafeClassID<Int32> { DJINN_COM_CLASSSTR("Int32"); };
            template<> struct SafeClassID<Int64> { DJINN_COM_CLASSSTR("Int64"); };
            template<> struct SafeClassID<UInt8> { DJINN_COM_CLASSSTR("UInt8"); };
            template<> struct SafeClassID<UInt16> { DJINN_COM_CLASSSTR("UInt16"); };
            template<> struct SafeClassID<UInt32> { DJINN_COM_CLASSSTR("UInt32"); };
            template<> struct SafeClassID<UInt64> { DJINN_COM_CLASSSTR("UInt64"); };
            template<> struct SafeClassID<float> { DJINN_COM_CLASSSTR("float"); };
            template<> struct SafeClassID<double> { DJINN_COM_CLASSSTR("double"); };
            template<> struct SafeClassID<ClassID> { DJINN_COM_CLASSSTR("COM.ClassID"); };
            template<> struct SafeClassID<InterfaceID> { DJINN_COM_CLASSSTR("COM.InterfaceID"); };

        }

        /// Get safe class id from type
        /// \tparam T
        template<typename T> inline constexpr Djinn::COM::ClassID SafeClassID = Detail::SafeClassID<typename Core::RemoveConstReference<T>::Type>::kCID;
    }

    /// Common hashes
    template<> struct Hasher<COM::ClassID> { USize operator()(const COM::ClassID& value) const { return Hasher<UInt64>{}(value.Get()); }};
    template<> struct Hasher<COM::InterfaceID> { USize operator()(const COM::InterfaceID& value) const { return Hasher<UInt64>{}(value.Get()); }};
    template<> struct Hasher<COM::InstanceID> { USize operator()(const COM::InstanceID& value) const { return Hasher<UInt64>{}(value.Get()); }};
}