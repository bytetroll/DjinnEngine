//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Types.h>
#include "TypeInfo.h"
#include "ParameterInfo.h"
#include <Core/DynamicDelegate.h>
#include <Core/ArrayView.h>
#include "IContainerInfo.h"

namespace Djinn::Reflection {
    class IAttribute;
    class IClassInfo;

    class IMethodInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the owning class
        /// \return
        virtual IClassInfo* GetClass() = 0;

        /// Get the return type
        /// \return
        virtual TypeInfo GetReturnType() = 0;

        /// Get all parameters
        /// \return
        virtual Core::ArrayView<ParameterInfo> GetParameters() = 0;

        /// Get the name
        /// \return
        virtual const char *GetName() = 0;

        /// Get the full decorated name
        /// \return
        virtual const char *GetDecoratedName() = 0;

        /// Get the full decorated signature
        /// \return
        virtual const char *GetDecoratedSignature() = 0;

        /// Get the signature hash
        /// \return
        virtual USize GetSignatureHash() = 0;

        /// Get the dynamic delegate representation from a given frame
        /// \param frame - May be nullptr
        /// \return
        virtual Core::DynamicDelegate GetDynamic(void *frame) = 0;

        /// Get the delegate representation from a given frame
        /// \param frame
        /// \return
        template<typename T>
        Core::Delegate<T> Get(void *frame) {
            return GetDynamic(frame).As<T>();
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo *>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IMethodInfo *>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}