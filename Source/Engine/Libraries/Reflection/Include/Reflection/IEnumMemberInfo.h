//
// Created by Dev on 5/29/2018.
//
#pragma once

#include "TypeInfo.h"
#include <Core/DynamicMember.h>
#include <COM/IUnknown.h>
#include <Core/ArrayView.h>
#include "IContainerInfo.h"

namespace Djinn::Reflection {
    class IAttribute;
    class IEnumInfo;

    class IEnumMemberInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the owning enum
        /// \return
        virtual IEnumInfo* GetEnum() = 0;

        /// Get the name
        /// \return
        virtual const char *GetName() = 0;

        /// Get the integer value of this enum member
        /// \return
        virtual Int64 GetValue() = 0;

        /// Get the hash
        /// \return
        virtual USize GetHash() = 0;

        /// Get the full decorated name
        /// \return
        virtual const char *GetDecoratedName() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo *>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IEnumMemberInfo *>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}