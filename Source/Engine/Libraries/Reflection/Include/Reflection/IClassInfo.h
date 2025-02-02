//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/String.h>
#include <Core/ArrayView.h>
#include <Core/StringView.h>
#include "ID.h"
#include "IContainerInfo.h"

namespace Djinn::Reflection {
    class IMemberInfo;
    class IMethodInfo;
    class IAttribute;

    class IClassInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the class id
        /// \return
        virtual ClassID GetReflectionID() = 0;

        /// Get all bases
        /// \return
        virtual Core::ArrayView<ClassID> GetBases() = 0;

        /// Is this a primitive type?
        /// \return
        virtual bool IsPrimitive() = 0;

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
        virtual void AddMemberNameProxy(IMemberInfo* member, USize proxy) = 0;

        /// Helper
        void AddMemberNameProxy(IMemberInfo* member, const Core::StringView& proxy) {
            return AddMemberNameProxy(member, proxy.Hash());
        }

        /// Add a name proxy for general querying
        /// \param from
        /// \param to
        virtual void AddMethodNameProxy(IMethodInfo* method, USize proxy) = 0;

        /// Helper
        void AddMethodNameProxy(IMethodInfo* method, const Core::StringView& proxy) {
            return AddMethodNameProxy(method, proxy.Hash());
        }

        /// Get member by name
        /// \param name
        /// \return
        IMemberInfo *GetMember(const char *name) {
            return GetMember(Core::ComputeCRC64(name));
        }

        /// Get member by name hash
        /// \param hash
        /// \return
        virtual IMemberInfo *GetMember(USize hash) = 0;

        /// Get method by name
        /// \param name
        /// \return
        IMethodInfo *GetMethod(const char *name) {
            return GetMethod(Core::ComputeCRC64(name));
        }

        /// Get method by name hash with optional signature hash
        /// \param hash
        /// \return
        virtual IMethodInfo *GetMethod(USize hash, USize signatureHash = 0) = 0;

        /// Get method by name hash and signature
        /// \param hash
        /// \return
        template<typename T>
        IMethodInfo *GetMethod(USize hash) {
            return GetMethod(hash, GetTypeNameCRC64<T>());
        }

        /// Get method by name and signature
        /// \param hash
        /// \return
        template<typename T>
        IMethodInfo *GetMethod(const char *name) {
            return GetMethod(Core::ComputeCRC64(name), GetTypeNameCRC64<T>());
        }

        /// Get method by name and signature hash
        /// \param hash
        /// \return
        IMethodInfo *GetMethod(const char *name, USize signatureHash) {
            return GetMethod(Core::ComputeCRC64(name), signatureHash);
        }

        /// Get all members
        /// \return
        virtual Core::ArrayView<IMemberInfo*> GetMembers() = 0;

        /// Get all methods
        /// \return
        virtual Core::ArrayView<IMethodInfo*> GetMethods() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IClassInfo*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}