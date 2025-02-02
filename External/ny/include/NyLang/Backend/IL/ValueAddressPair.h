//
// Created by Dev on 12/6/2017.
//
#pragma once

namespace NyLang::IL {
    class IValue;

    struct ValueAddressPair {
        ValueAddressPair(std::nullptr_t = nullptr) : value(nullptr), address(nullptr) {};
        explicit ValueAddressPair(IValue* value, IValue* address = nullptr) : value(value), address(address) {
            // ...
        }

        operator bool() const {
            return value || address;
        }

        operator IValue*() const {
            return value ?: address;
        }

        IValue* operator->() const {
            return value ?: address;
        }

        IValue* value = nullptr;
        IValue* address = nullptr;
    };
}