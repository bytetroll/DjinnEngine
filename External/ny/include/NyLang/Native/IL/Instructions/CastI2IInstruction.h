//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class CastI2IInstruction : public IInstruction {
    public:
        LANG_CLASSID("CastI2IInstruction");

        /// Construct
        /// \param operands
        CastI2IInstruction(Context& context, IValue* lhs, int bitWidth, bool isUnsigned);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetValue() { return GetOperand(0); }
        int GetBitWidth() { return bitWidth; }
        bool IsUnsigned() { return isUnsigned; }

    private:
        int bitWidth;
        bool isUnsigned;
    };
}
