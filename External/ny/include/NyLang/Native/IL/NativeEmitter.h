//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Backend/IL/Emitter.h>
#include <NyLang/Backend/IL/Block.h>
#include <NyLang/Backend/IL/ValueAddressPair.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class Block;
    class IValue;
    class IType;
    class Member;
    class Function;
    class Class;

    class NativeEmitter {
    public:
        NativeEmitter(Context& context, Emitter*emitter) : emitter(emitter), context(context) {

        }

        /// Set current block
        /// \param block
        void SetCurrentBlock(Block*block) {
            emitter->SetCurrentBlock(block);
        }

        /// Get current block
        Block* GetCurrentBlock() {
            return emitter->GetCurrentBlock();
        }

        /// Set current container
        /// \param block
        void SetCurrentContainer(Container*container) {
            emitter->SetCurrentContainer(container);
        }

        /// Get current container
        Container* GetCurrentContainer() {
            return emitter->GetCurrentContainer();
        }

        /// Create new block in current execution block
        /// \return
        Block* CreateBlock();

        /// Create add instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateAdd(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create stack allocation instruction
        /// \param type
        /// \return
        ValueAddressPair CreateAlloca(IType*type);

        /// Create bitwise and instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBitAnd(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create bitwise left shift instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBitLShift(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create bitwise not instruction
        /// \param value
        /// \return
        ValueAddressPair CreateBitNot(const ValueAddressPair &value);

        /// Create negation instruction
        /// \param value
        /// \return
        ValueAddressPair CreateNeg(const ValueAddressPair &value);

        /// Create bitwise or instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBitOr(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create bitwise right shift instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBitRShift(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create bitwise xor instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBitXor(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create call instruction
        /// \param function
        /// \param arguments
        /// \return
        ValueAddressPair CreateCall(const ValueAddressPair &function, const PtrVector<IValue> &arguments);

        /// Create cast float to float instruction
        /// \param value
        /// \param bitWidth
        /// \param isUnsigned
        /// \return
        ValueAddressPair CreateCastF2I(const ValueAddressPair &value, int bitWidth, bool isUnsigned);

        /// Create cast integer to float instruction
        /// \param lhs
        /// \param bitWidth
        /// \return
        ValueAddressPair CreateCastI2F(const ValueAddressPair &lhs, int bitWidth);

        /// Create cast integer to integer instruction
        /// \param lhs
        /// \param bitWidth
        /// \param isUnsigned
        /// \return
        ValueAddressPair CreateCastI2I(const ValueAddressPair &lhs, int bitWidth, bool isUnsigned);

        /// Create cast float to float instruction
        /// \param value
        /// \param bitWidth
        /// \return
        ValueAddressPair CreateCastF2F(const ValueAddressPair &value, int bitWidth);

        /// Create compare equal instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpEq(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create compare greater equal instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpGe(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create compare greater than instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpGt(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create compare less equal instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpLe(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create compare less than instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpLt(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create compare not equal instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateCmpNeq(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create div instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateDiv(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create element address instruction
        /// \param address
        /// \param index
        /// \return
        ValueAddressPair CreateElementAddress(const ValueAddressPair &address, const ValueAddressPair &index);

        /// Create element instruction
        /// \param address
        /// \param index
        /// \return
        ValueAddressPair CreateElement(const ValueAddressPair &array, const ValueAddressPair &index);


        /// Create field address instruction
        /// \param address
        /// \param member
        /// \return
        ValueAddressPair CreateFieldAddress(const ValueAddressPair &address, Member*member);

        /// Create field address instruction
        /// \param address
        /// \param member
        /// \return
        ValueAddressPair CreateField(const ValueAddressPair &value, Member*member);

        /// Create function address instruction
        /// \param value
        /// \param member
        /// \return
        ValueAddressPair CreateFunctionAddress(const ValueAddressPair &value, Function*member);

        /// Create load instruction
        /// \param address
        /// \return
        ValueAddressPair CreateLoad(const ValueAddressPair &address);

        /// Create store instruction
        /// \param address
        /// \param value
        /// \return
        ValueAddressPair CreateStore(const ValueAddressPair &address, const ValueAddressPair &value);

        /// Create mod instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateMod(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create return instruction
        /// \param value
        /// \return
        ValueAddressPair CreateReturn(const ValueAddressPair &value);

        /// Create sub instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateSub(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create mul instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateMul(const ValueAddressPair &lhs, const ValueAddressPair &rhs);

        /// Create this instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateThis(Class*_class);

        /// Create br instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBr(Block*block);

        /// Create brcond instruction
        /// \param lhs
        /// \param rhs
        /// \return
        ValueAddressPair CreateBrCond(const ValueAddressPair& cond, Block*_true, Block* _false);

        /// Create instruction
        /// \tparam T
        /// \tparam A
        /// \param args
        /// \return
        template<typename T, typename... A>
        T* Create(A &&...args) {
            auto instr = context.New<T>(args...);
            emitter->GetCurrentBlock()->AddInstruction(instr);
            return instr;
        }

    private:
        Context& context;

        // ...
        Emitter* emitter = nullptr;
    };
}
