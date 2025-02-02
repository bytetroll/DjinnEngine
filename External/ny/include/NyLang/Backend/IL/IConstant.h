//
// Created by Dev on 12/6/2017.
//
#pragma once

#include "IValue.h"

namespace NyLang::IL {
    class IConstant : public IValue {
    public:
        IConstant(IType*type);
    };
}