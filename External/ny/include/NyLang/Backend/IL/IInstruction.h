//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/StdTypes.h>
#include "IValue.h"

namespace NyLang::IL {
    class Block;

    class IInstruction : public IValue {
    public:
        IInstruction(IType* type, const PtrVector<IValue>& operands);

        /// Add operand
        /// \param operand
        void AddOperand(IValue* operand);

        /// Get operands
        /// \return
        const auto& GetOperands() const {
            return operands;
        }

        /// Get operand
        /// \param i
        /// \return
        IValue* GetOperand(std::size_t i) const {
            return operands[i];
        }

        /// Set block
        /// \param block
        void SetBlock(Block* block) {
            this->block = block;
        }

        /// Get block
        /// \return
        Block* GetBlock() const {
            return block;
        }

        /// Release uses
        void ReleaseUses();

        /// Is this instruction immutable?
        /// \return
        bool IsImmutable() const {
            return isImmutable;
        }

    protected:
        PtrVector<IValue> operands;

        // Super block
        Block* block = nullptr;

        // Is this instruction immutable?
        // I.e. does it perform any external state modification?
        bool isImmutable = false;
    };
}
