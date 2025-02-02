//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>

namespace NyLang::IL {
    class Member;

    class FieldInstruction : public IInstruction {
    public:
        LANG_CLASSID("FieldInstruction");

        /// Construct
        /// \param operands
        FieldInstruction(IValue* value, Member* member);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetValue() { return GetOperand(0); }
        Member*GetMember() { return member; }

    private:
        Member* member = nullptr;
    };
}
