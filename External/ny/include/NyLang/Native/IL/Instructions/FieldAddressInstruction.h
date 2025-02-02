//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class Member;

    class FieldAddressInstruction : public IInstruction {
    public:
        LANG_CLASSID("FieldAddressInstruction");

        /// Construct
        /// \param operands
        FieldAddressInstruction(Context& context, IValue* address, IValue* member);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetAddress() { return GetOperand(0); }
        IValue*GetMember() { return GetOperand(1); }
    };
}
