//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ElementAddressInstruction : public IInstruction {
    public:
        LANG_CLASSID("ElementAddressInstruction");

        /// Construct
        /// \param operands
        ElementAddressInstruction(Context& context, IValue* address, IValue* index);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IValue*GetArray() { return GetOperand(0); }
        IValue*GetIndex() { return GetOperand(1); }
    };
}
