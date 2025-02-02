//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/PrettyStream.h>
#include "Block.h"
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class ExecutionBlock {
    public:
        /// Pretty print
        /// \param stream
        void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context);

        /// Add a block
        /// \param block
        void AddBlock(Block* block) {
            blocks.emplace_back(block);
            block->SetParent(this);
        }

        /// Get blocks
        /// \return
        PtrVector<Block>& GetBlocks() {
            return blocks;
        }

    private:
        PtrVector<Block> blocks;
    };
}
