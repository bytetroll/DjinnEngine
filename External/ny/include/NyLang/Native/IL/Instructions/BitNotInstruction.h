//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>

namespace NyLang::IL {
    class BitNotInstruction : public IInstruction {
    public:
        LANG_CLASSID("BitNotInstruction");

        /// Construct
        /// \param operands
        BitNotInstruction(IValue* value);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetValue() { return GetOperand(0); }
    };
}
