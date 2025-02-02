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
    class IClassInfo;

    class IMemberInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the owning class
        /// \return
        virtual IClassInfo* GetClass() = 0;

        /// Get the type
        /// \return
        virtual const TypeInfo &GetType() = 0;

        /// Get the name
        /// \return
        virtual const char *GetName() = 0;

        /// Get the hash
        /// \return
        virtual USize GetHash() = 0;

        /// Get the full decorated name
        /// \return
        virtual const char *GetDecoratedName() = 0;

        /// Get the dynamic member representation from a given frame
        /// \param frame - May be nullptr
        /// \return
        virtual Core::DynamicMember GetDynamic(void *frame) = 0;

        /// Get the component representation from a given frame
        /// \param frame
        /// \return - Nullptr if not convertible
        virtual COM::IUnknown *GetCom(void *frame) = 0;

        /// Get raw pointer from a given frame
        /// \param frame
        /// \return
        virtual void *GetRaw(void *frame) = 0;

        /// Convert this member to string
        /// \param frame
        /// \return
        virtual Core::String ToString(void *frame) = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo *>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IMemberInfo *>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}