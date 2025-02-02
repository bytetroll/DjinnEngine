//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ReturnInstruction : public IInstruction {
    public:
        LANG_CLASSID("ReturnInstruction");

        /// Construct
        /// \param operands
        ReturnInstruction(Context& context, IValue* value);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetValue() { return GetOperand(0); }
    };
}
