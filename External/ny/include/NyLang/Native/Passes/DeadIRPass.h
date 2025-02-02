//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Backend/Pass/IPass.h>

namespace NyLang::IL {
    class ExecutionBlock;

    class DeadIRPass : public IPass {
    public:
        LANG_CLASSID("DeadIRPass");

        void Initialize(bool dropMembers = true);

        /// Overrides
        bool Pass(Program*program) override;
        virtual ClassID GetClassID() override;

    private:
        bool dropMembers = true;
        void PassExecutionBlock(ExecutionBlock* executionBlock, bool& mutated);
    };
}
