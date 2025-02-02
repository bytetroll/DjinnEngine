//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class BrInstruction : public IInstruction {
    public:
        LANG_CLASSID("BrInstruction");

        /// Construct
        /// \param operands
        BrInstruction(Context& context, Block* block);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Getters
        Block*GetBlock() { return block; }

    private:
        Block* block = nullptr;
    };
}
