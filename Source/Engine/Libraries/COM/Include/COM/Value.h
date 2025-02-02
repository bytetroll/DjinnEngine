//
// Created by Dev on 10/21/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::COM {
    /// Value wrapper
    template<typename T>
    class Value : public COM::IUnknown {
    public:
        Value(T* value) : COM::IUnknown(nullptr, nullptr), value(value) {

        }

        virtual COM::ClassID GetClassID() override {
            return COM::SafeClassID<T>;
        }

        T* value;
    };
}