//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>

namespace NyLang::IL {
    class CallInstruction : public IInstruction {
    public:
        LANG_CLASSID("CallInstruction");

        /// Construct
        /// \param operands
        CallInstruction(IValue* function, const PtrVector<IValue>& arguments);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetFunction() { return GetOperand(0); }
        auto GetArgIt() { return ++GetOperands().begin(); }
    };
}
