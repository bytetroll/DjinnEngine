//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class CmpGtInstruction : public IInstruction {
    public:
        LANG_CLASSID("CmpGtInstruction");

        /// Construct
        /// \param operands
        CmpGtInstruction(Context& context, IValue* lhs, IValue* rhs);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetLHS() { return GetOperand(0); }
        IValue*GetRHS() { return GetOperand(1); }
    };
}
