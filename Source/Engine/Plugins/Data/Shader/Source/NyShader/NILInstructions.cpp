#include <Data/NyShader/NILInstructions.h>
#include <NyLang/Backend/IL/IType.h>
#include <Core/Common.h>
#include <NyLang/Native/IL/Types/FPType.h>
#include <NyLang/Native/IL/Types/IntegerType.h>

DJINN_NS3(Data, NyShader);

VectorSetInstruction::VectorSetInstruction(NyLang::IL::IType *resultType, const NyLang::PtrVector<NyLang::IL::IValue> &values) : IInstruction(resultType, values) {

}

void VectorSetInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectorset ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    for (auto it = operands.begin(); it != operands.end(); it++) {
        if (it != operands.begin()) {
            stream << ", ";
        }
        (*it)->PrettyPrintReference(stream, context);
    }
}

NyLang::ClassID VectorSetInstruction::GetClassID() {
    return kCID;
}

VectorAddInstruction::VectorAddInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void VectorAddInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectoradd ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorAddInstruction::GetClassID() {
    return kCID;
}

VectorLengthInstruction::VectorLengthInstruction(NyLang::Context& context, NyLang::IL::IValue *v) : IInstruction(context.New<NyLang::IL::FPType>(32), {v}) {

}

void VectorLengthInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectorlength ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorLengthInstruction::GetClassID() {
    return kCID;
}

VectorLength2Instruction::VectorLength2Instruction(NyLang::Context& context, NyLang::IL::IValue *v) : IInstruction(context.New<NyLang::IL::FPType>(32), {v}) {

}

void VectorLength2Instruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectorlength2 ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorLength2Instruction::GetClassID() {
    return kCID;
}

VectorSumInstruction::VectorSumInstruction(NyLang::Context& context, NyLang::IL::IValue *v): IInstruction(context.New<NyLang::IL::FPType>(32), {v}) {

}

void VectorSumInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "dsf_vectorsum ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID VectorSumInstruction::GetClassID() {
    return kCID;
}

VectorSubInstruction::VectorSubInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void VectorSubInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectorsub ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorSubInstruction::GetClassID() {
    return kCID;
}

VectorMulInstruction::VectorMulInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void VectorMulInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectormul ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorMulInstruction::GetClassID() {
    return kCID;
}

VectorDivInstruction::VectorDivInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void VectorDivInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_vectordiv ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID VectorDivInstruction::GetClassID() {
    return kCID;
}

MatrixSetInstruction::MatrixSetInstruction(NyLang::IL::IType *resultType, const NyLang::PtrVector<NyLang::IL::IValue> &values) : IInstruction(resultType, values) {

}

void MatrixSetInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_matrixset ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    for (auto it = operands.begin(); it != operands.end(); it++) {
        if (it != operands.begin()) {
            stream << ", ";
        }
        (*it)->PrettyPrintReference(stream, context);
    }
}

NyLang::ClassID MatrixSetInstruction::GetClassID() {
    return kCID;
}

MatrixMulInstruction::MatrixMulInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void MatrixMulInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_matrixmul ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID MatrixMulInstruction::GetClassID() {
    return kCID;
}

MatrixTransformInstruction::MatrixTransformInstruction(NyLang::IL::IValue *lhs, NyLang::IL::IValue *rhs) : IInstruction(lhs->GetType(), {lhs, rhs}) {

}

void MatrixTransformInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "dsf_matrixtransform ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID MatrixTransformInstruction::GetClassID() {
    return kCID;
}

TextureSampleGradientInstruction::TextureSampleGradientInstruction(
        NyLang::IL::IValue *texture, NyLang::IL::IType *resultType, NyLang::IL::IValue *texcoord
) : IInstruction(resultType, {texture, texcoord}) {

}

void TextureSampleGradientInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "texturesamplegradient ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID TextureSampleGradientInstruction::GetClassID() {
    return kCID;
}

TextureSampleTexelInstruction::TextureSampleTexelInstruction(
        NyLang::IL::IValue *texture, NyLang::IL::IType *resultType, NyLang::IL::IValue *texcoord
) : IInstruction(resultType, {texture, texcoord}) {

}

void TextureSampleTexelInstruction::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    stream << "texturesamplegradient ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

NyLang::ClassID TextureSampleTexelInstruction::GetClassID() {
    return kCID;
}

SmoothstepInstruction::SmoothstepInstruction(IValue *a, IValue *b, IValue *t) : IInstruction(a->GetType(), {a, b, t}) {

}

void SmoothstepInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "smoothstep ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(2)->PrettyPrintReference(stream, context);
}

ClassID SmoothstepInstruction::GetClassID() {
    return kCID;
}

VectorSwizzleInstruction::VectorSwizzleInstruction(IL::IType *resultType, IValue *vector, const Core::FixedArray4<int> &coms)
        : IInstruction(resultType, {vector}), coms(coms) {

}

void VectorSwizzleInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "vectorswizzle ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    for (auto& com : coms) {
        stream << com << ":";
    }
}

ClassID VectorSwizzleInstruction::GetClassID() {
    return kCID;
}

ClampInstruction::ClampInstruction(IValue *v, IValue *a, IValue *b) : IInstruction(v->GetType(), {v, a, b}) {

}

void ClampInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "clamp ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(2)->PrettyPrintReference(stream, context);
}

ClassID ClampInstruction::GetClassID() {
    return kCID;
}

MinInstruction::MinInstruction(IValue *a, IValue *b) : IInstruction(a->GetType(), {a, b}) {

}

void MinInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "min ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

ClassID MinInstruction::GetClassID() {
    return kCID;
}

MaxInstruction::MaxInstruction(IValue *a, IValue *b) : IInstruction(a->GetType(), {a, b}) {

}

void MaxInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "min ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

ClassID MaxInstruction::GetClassID() {
    return kCID;
}

AbsInstruction::AbsInstruction(IValue *a) : IInstruction(a->GetType(), {a}) {

}

void AbsInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "abs ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID AbsInstruction::GetClassID() {
    return kCID;
}

SqrtInstruction::SqrtInstruction(NyLang::IL::IValue *a) : IInstruction(a->GetType(), {a}) {

}

void SqrtInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "sqrt ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID SqrtInstruction::GetClassID() {
    return kCID;
}

AsFloatInstruction::AsFloatInstruction(NyLang::Context& context, NyLang::IL::IValue *a) : IInstruction(context.New<NyLang::IL::FPType>(32), {a}) {

}

void AsFloatInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "asfloat ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID AsFloatInstruction::GetClassID() {
    return kCID;
}

AsUIntInstruction::AsUIntInstruction(NyLang::Context& context, NyLang::IL::IValue *a) : IInstruction(context.New<NyLang::IL::IntegerType>(32, true), {a}) {

}

void AsUIntInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "asuint ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID AsUIntInstruction::GetClassID() {
    return kCID;
}

AsIntInstruction::AsIntInstruction(NyLang::Context& context, NyLang::IL::IValue *a) : IInstruction(context.New<NyLang::IL::IntegerType>(32, false), {a}) {

}

void AsIntInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "asint ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
}

ClassID AsIntInstruction::GetClassID() {
    return kCID;
}

LerpInstruction::LerpInstruction(IValue *a, IValue *b, IValue *t) : IInstruction(a->GetType(), {a, b, t}) {

}

void LerpInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "lerp ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(2)->PrettyPrintReference(stream, context);
}

ClassID LerpInstruction::GetClassID() {
    return kCID;
}

DotInstruction::DotInstruction(IValue *a, IValue *b) : IInstruction(a->GetType(), {a, b}) {

}

void DotInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "lerp ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

ClassID DotInstruction::GetClassID() {
    return kCID;
}

DistanceInstruction::DistanceInstruction(IValue *a, IValue *b) : IInstruction(a->GetType(), {a, b}) {

}

void DistanceInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "distance ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

ClassID DistanceInstruction::GetClassID() {
    return kCID;
}

CrossInstruction::CrossInstruction(IValue *a, IValue *b) : IInstruction(a->GetType(), {a, b}) {

}

void CrossInstruction::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "cross ";
    GetType()->PrettyPrint(stream, context);
    stream << " ";
    GetOperand(0)->PrettyPrintReference(stream, context);
    stream << " ";
    GetOperand(1)->PrettyPrintReference(stream, context);
}

ClassID CrossInstruction::GetClassID() {
    return kCID;
}
