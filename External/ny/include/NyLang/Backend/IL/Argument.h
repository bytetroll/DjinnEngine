//
// Created by Dev on 12/4/2017.
//
#pragma once

#include "IValue.h"

namespace NyLang::IL {
    class Argument : public IValue {
    public:
        LANG_CLASSID("Argument");

        /// Construct
        /// \param type
        Argument(IType*type);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;
    };
}