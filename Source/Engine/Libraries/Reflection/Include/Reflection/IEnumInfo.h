//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/String.h>
#include <Core/ArrayView.h>
#include <Core/StringView.h>
#include <Core/Pair.h>
#include "ID.h"
#include "TypeInfo.h"
#include "IContainerInfo.h"

namespace Djinn::Reflection {
    class IAttribute;
    class IEnumMemberInfo;

    class IEnumInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the class id
        /// \return
        virtual ClassID GetReflectionID() = 0;

        /// Get all enum members
        /// \return
        virtual Core::ArrayView<IEnumMemberInfo *> GetMembers() = 0;

        /// Get the underlying type
        /// \return
        virtual const TypeInfo &GetUnderlyingType() = 0;

        /// Get the actual target byte size
        /// \return
        virtual USize GetByteSize() = 0;

        /// Get the name
        /// \return
        virtual const char *GetName() = 0;

        /// Get the full qualified name
        /// \return
        virtual const char *GetQualifiedName() = 0;

        /// Get the full decorated name
        /// \return
        virtual const char *GetDecoratedName() = 0;

        /// Add a name proxy for general querying
        /// \param from
        /// \param to
        virtual void AddNameProxy(IEnumMemberInfo* member, USize proxy) = 0;

        /// Helper
        void AddNameProxy(IEnumMemberInfo* member, const Core::StringView& proxy) {
            return AddNameProxy(member, proxy.Hash());
        }

        /// Get member by name
        /// \param name
        /// \return
        IEnumMemberInfo *GetMember(const char *name) {
            return GetMember(Core::ComputeCRC64(name));
        }

        /// Get member by name hash
        /// \param hash
        /// \return
        virtual IEnumMemberInfo *GetMember(USize hash) = 0;

        /// Convert an enum value to a string
        /// \param value
        /// \param out
        /// \return
        virtual bool ToString(Int64 value, Core::StringView &out) = 0;

        /// Helper for "guarenteed" enum values
        /// \param value
        /// \return
        template<typename T>
        Core::StringView ToString(T value) {
            Core::StringView view;
            if (!ToString(static_cast<Int64>(value), view)) {
                return {};
            }
            return view;
        }

        /// Convert an enum name hash to the respective enum value
        /// \param hash
        /// \param out
        /// \return - false if not a valid member
        virtual bool ToIndex(USize hash, Int64 &out) = 0;

        /// Convert an enum name to the respective enum value
        /// \param name
        /// \param out
        /// \return - false if not a valid member
        bool ToIndex(const Core::StringView &name, Int64 &out) {
            return ToIndex(name.Hash(), out);
        }

        /// Convert an enum hash to the respective enum value of type
        /// \param name
        /// \param out
        /// \return
        template<typename T>
        bool ToIndex(USize hash, T &out) {
            Int64 value;
            if (!ToIndex(hash, value)) {
                return false;
            }

            out = static_cast<T>(value);
            return true;
        }

        /// Convert an enum name to the respective enum value of type
        /// \param name
        /// \param out
        /// \return
        template<typename T>
        bool ToIndex(const Core::StringView &name, T &out) {
            Int64 value;
            if (!ToIndex(name.Hash(), value)) {
                return false;
            }

            out = static_cast<T>(value);
            return true;
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IEnumInfo*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}