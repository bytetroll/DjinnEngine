//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/PrettyStream.h>
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class IInstruction;
    class ExecutionBlock;

    class Block {
    public:
        /// Pretty print
        /// \param stream
        void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context);

        /// Add instruction
        /// \param instr
        void AddInstruction(IInstruction* instr);

        /// Add instruction before iterator
        /// \param before
        /// \param instr
        void AddInstruction(const PtrList<IInstruction>::iterator& before, IInstruction* instr);

        /// Remove instruction
        /// \param instr
        void RemoveInstruction(IInstruction* instr);

        /// Get instructions
        /// \return
        PtrList<IInstruction>& GetInstructions() {
            return instructions;
        }

        /// Merge with other block, will take ownership of instructions
        /// \param other
        void Merge(Block* other);

        /// Set parent
        /// \param parent
        void SetParent(ExecutionBlock* parent) {
            this->parent = parent;
        }

        /// Get parent
        /// \return
        ExecutionBlock* GetParent() const {
            return parent;
        }

    private:
        PtrList<IInstruction> instructions;

        // Parent
        ExecutionBlock* parent = nullptr;
    };
}
