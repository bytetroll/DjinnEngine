//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>

namespace NyLang::IL {
    class DivInstruction : public IInstruction {
    public:
        LANG_CLASSID("DivInstruction");

        /// Construct
        /// \param operands
        DivInstruction(IValue* lhs, IValue* rhs);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetLHS() { return GetOperand(0); }
        IValue*GetRHS() { return GetOperand(1); }
    };
}
