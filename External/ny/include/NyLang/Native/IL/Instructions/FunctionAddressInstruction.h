//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>

namespace NyLang::IL {
    class Function;

    class FunctionAddressInstruction : public IInstruction {
    public:
        LANG_CLASSID("FunctionAddressInstruction");

        /// Construct
        /// \param operands
        FunctionAddressInstruction(Context& context, IValue* value, Function* member);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetAddress() { return GetOperand(0); }
        Function*GetFunction() { return function; }

    private:
        Function* function = nullptr;
    };
}
