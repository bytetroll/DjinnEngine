//
// Created by Dev on 5/29/2018.
//
#pragma once

#include "TypeInfo.h"

namespace Djinn::Reflection {
    class ParameterInfo {
    public:
        ParameterInfo(TypeInfo type, const char* name) : type(type), name(name) {
            decoratedName = type.GetDecoratedName() + Core::String(" ") + Core::String(name);
        }

        const TypeInfo& GetType() const {
            return type;
        }

        const char *GetName() const {
            return name.AsCharArray();
        }

        const char *GetDecoratedName() const {
            return decoratedName.AsCharArray();
        }

        TypeInfo type;
        Core::String name;
        Core::String decoratedName;
    };
}