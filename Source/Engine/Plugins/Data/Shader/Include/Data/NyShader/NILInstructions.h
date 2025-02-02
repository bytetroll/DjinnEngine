//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <NyLang/Backend/IL/IInstruction.h>
#include <Core/FixedArray.h>
#include <NyLang/Core/Context.h>

namespace Djinn::Data::NyShader {
    using namespace NyLang;

    class VectorSetInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorSetInstruction");

        /// Construct
        /// \param resultType
        /// \param values
        VectorSetInstruction(IL::IType *resultType, const PtrVector<IValue> &values);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IType * GetResultType() { return GetType(); }
        const auto& GetValues() { return GetOperands(); }
    };

    class VectorSwizzleInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorSwizzleInstruction");

        /// Construct
        /// \param resultType
        /// \param values
        VectorSwizzleInstruction(IL::IType *resultType, IValue *vector, const Core::FixedArray4<int>& coms);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IType * GetResultType() { return GetType(); }
        IValue * GetVector() { return GetOperand(0); }
        Core::FixedArray4<int> coms;
    };

    class VectorLengthInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorLengthInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorLengthInstruction(NyLang::Context& context, IValue *v);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetV() { return GetOperand(0); }
    };

    class VectorLength2Instruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorLength2Instruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorLength2Instruction(NyLang::Context& context, IValue *v);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetV() { return GetOperand(0); }
    };

    class VectorSumInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorSumInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorSumInstruction(NyLang::Context& context, IValue *v);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetV() { return GetOperand(0); }
    };

    class VectorAddInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorAddInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorAddInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class VectorSubInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorSubInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorSubInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class VectorMulInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorMulInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorMulInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class VectorDivInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("VectorDivInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        VectorDivInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class MatrixSetInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("MatrixSetInstruction");

        /// Construct
        /// \param resultType
        /// \param values
        MatrixSetInstruction(IL::IType *resultType, const PtrVector<IValue> &values);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IType * GetResultType() { return GetType(); }
        const auto& GetValues() { return GetOperands(); }
    };

    class MatrixMulInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("MatrixMulInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        MatrixMulInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class MatrixTransformInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("MatrixTransformInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        MatrixTransformInstruction(IValue *lhs, IValue *rhs);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetLHS() { return GetOperand(0); }
        IL::IValue * GetRHS() { return GetOperand(1); }
    };

    class TextureSampleGradientInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("TextureSampleGradientInstruction");

        /// Construct
        /// \param texture
        /// \param resultType
        /// \param texcoord
        TextureSampleGradientInstruction(IValue *texture, IL::IType *resultType, IValue *texcoord);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IType * GetResultType() { return GetType(); }
        IL::IValue * GetTexture() { return GetOperand(0); }
        IL::IValue * GetTexcoord() { return GetOperand(1); }
    };

    class TextureSampleTexelInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("TextureSampleTexelInstruction");

        /// Construct
        /// \param texture
        /// \param resultType
        /// \param texcoord
        TextureSampleTexelInstruction(IValue *texture, IL::IType *resultType, IValue *texcoord);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IType * GetResultType() { return GetType(); }
        IL::IValue * GetTexture() { return GetOperand(0); }
        IL::IValue * GetTexcoord() { return GetOperand(1); }
    };

    class SmoothstepInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("SmoothstepInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        SmoothstepInstruction(IValue *a, IValue *b, IValue *t);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
        IL::IValue * GetT() { return GetOperand(2); }
    };

    class ClampInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("ClampInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        ClampInstruction(IValue *a, IValue *b, IValue *t);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetV() { return GetOperand(0); }
        IL::IValue * GetA() { return GetOperand(1); }
        IL::IValue * GetB() { return GetOperand(2); }
    };

    class MaxInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("MaxInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        MaxInstruction(IValue *a, IValue *b);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
    };

    class MinInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("MinInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        MinInstruction(IValue *a, IValue *b);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
    };

    class AbsInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("AbsInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        AbsInstruction(IValue *a);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
    };

    class SqrtInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("SqrtInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        SqrtInstruction(IValue *a);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
    };

    class AsFloatInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("AsFloatInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        AsFloatInstruction(NyLang::Context& context, IValue *a);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
    };

    class AsUIntInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("AsUIntInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        AsUIntInstruction(NyLang::Context& context, IValue *a);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
    };

    class AsIntInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("AsIntInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        AsIntInstruction(NyLang::Context& context, IValue *a);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
    };

    class LerpInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("LerpInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        LerpInstruction(IValue *a, IValue *b, IValue *t);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
        IL::IValue * GetT() { return GetOperand(2); }
    };

    class DotInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("DotInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        DotInstruction(IValue *a, IValue *b);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
    };

    class DistanceInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("DistanceInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        DistanceInstruction(IValue *a, IValue *b);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
    };

    class CrossInstruction : public IL::IInstruction {
    public:
        LANG_CLASSID("CrossInstruction");

        /// Construct
        /// \param lhs
        /// \param rhs
        CrossInstruction(IValue *a, IValue *b);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        /// Getters
        IL::IValue * GetA() { return GetOperand(0); }
        IL::IValue * GetB() { return GetOperand(1); }
    };
}