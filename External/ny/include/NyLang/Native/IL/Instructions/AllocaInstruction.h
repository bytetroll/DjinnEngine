//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class AllocaInstruction : public IInstruction {
    public:
        LANG_CLASSID("AllocaInstruction");

        /// Construct
        /// \param operands
        AllocaInstruction(Context& context, IType* type);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        IType*GetAllocaType() { return contained; }

    private:
        IType* contained = nullptr;
    };
}
