//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Size.h>
#include <COM/IUnknown.h>
#include <Core/ArrayView.h>
#include "ID.h"

namespace Djinn::Reflection {
    class IAttribute;

    class IContainerInfo : public COM::IQueryable {
    public:
        DJINN_COM_INTERFACEID();

        /// Get attributes
        /// \return
        virtual Core::ArrayView<IAttribute *> GetAttributes() = 0;

        /// Get attribute with class id
        /// \param id
        /// \return
        virtual IAttribute *GetAttribute(const ClassID &id) = 0;

        /// Get attribute from type
        /// \return
        template<typename T>
        T *GetAttribute() {
            return static_cast<T *>(GetAttribute(T::kCID));
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case kIID:
                    *out = static_cast<IContainerInfo*>(this);
                    break;
            }
            return COM::kUnknownInterface;
        }
    };
}