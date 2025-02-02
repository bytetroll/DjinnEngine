//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class BrCondInstruction : public IInstruction {
    public:
        LANG_CLASSID("BrCondInstruction");

        /// Construct
        /// \param operands
        BrCondInstruction(Context& context, IValue* condition, Block* _true, Block* _false);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetCondition() { return GetOperand(0); }
        Block*GetTrueBlock() { return _true; }
        Block*GetFalseBlock() { return _false; }

    private:
        Block* _true = nullptr;
        Block* _false = nullptr;
    };
}
