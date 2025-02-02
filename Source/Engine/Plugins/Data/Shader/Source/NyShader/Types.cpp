#include <Data/NyShader/Types.h>
#include <Data/NyShader/NILTypes.h>
#include <Core/Common.h>

DJINN_NS3(Data, NyShader);

SymbolTypeVector::SymbolTypeVector(Context& context, NyLang::SymbolTypeInfo* TypeInfo, unsigned CountX) : SymbolType(1, context.New<VectorType>(TypeInfo->il, CountX)), ContainedTypeInfo(TypeInfo), CountX(CountX) {
    // ...
}

SymbolTypeVector::SymbolTypeVector(Context& context, SymbolTypeVector *other) : SymbolTypeVector(context, other->ContainedTypeInfo, other->CountX) {
    // ...
}

void SymbolTypeVector::PrettyPrint(NyLang::PrettyStream &stream) {
    stream << "vec_";
    ContainedTypeInfo->PrettyPrint(stream);
    stream << "_" << CountX;
}

bool SymbolTypeVector::Compare(NyLang::SymbolComponent * other) {
    if (kCID != other->GetClassID()) {
        return false;
    }

    // ...
    auto vector = static_cast<SymbolTypeVector *>(other);

    // Compare contained and count
    if (!ContainedTypeInfo->Compare(vector->ContainedTypeInfo) || CountX != vector->CountX) {
        return false;
    }

    // Ok
    return true;
}

NyLang::SymbolType * SymbolTypeVector::PatternReplace(Context& context, std::list<NyLang::SymbolType *> &) {
    return nullptr;
}

bool SymbolTypeVector::IsIncomplete() {
    return ContainedTypeInfo->IsIncomplete();
}

NyLang::SymbolComponent * SymbolTypeVector::Copy(Context& context) {
    return context.New<SymbolTypeVector>(context, this);
}

NyLang::ClassID SymbolTypeVector::GetClassID() {
    return kCID;
}

SymbolTypeMatrix::SymbolTypeMatrix(Context& context, NyLang::SymbolTypeInfo *TypeInfo, unsigned CountX, unsigned CountY) : NyLang::SymbolType(1, context.New<MatrixType>(TypeInfo->il, CountX, CountY)), ContainedTypeInfo(TypeInfo),
                                                                                                                                CountX(CountX), CountY(CountY) {
    // ...
}

SymbolTypeMatrix::SymbolTypeMatrix(Context& context, SymbolTypeMatrix *other) : SymbolTypeMatrix(context, other->ContainedTypeInfo, other->CountX, other->CountY) {
    // ...
}

void SymbolTypeMatrix::PrettyPrint(NyLang::PrettyStream &stream) {
    stream << "mat_";
    ContainedTypeInfo->PrettyPrint(stream);
    stream << "_" << CountX << "x" << CountY;
}

bool SymbolTypeMatrix::Compare(NyLang::SymbolComponent * other) {
    if (kCID != other->GetClassID()) {
        return false;
    }

    // ...
    auto vector = static_cast<SymbolTypeMatrix *>(other);

    // Compare contained and count
    if (!ContainedTypeInfo->Compare(vector->ContainedTypeInfo) || CountX != vector->CountX || CountY != vector->CountY) {
        return false;
    }

    // Ok
    return true;
}

NyLang::SymbolType * SymbolTypeMatrix::PatternReplace(Context& context, std::list<NyLang::SymbolType *> &) {
    return nullptr;
}

bool SymbolTypeMatrix::IsIncomplete() {
    return ContainedTypeInfo->IsIncomplete();
}

NyLang::SymbolComponent * SymbolTypeMatrix::Copy(Context& context) {
    return context.New<SymbolTypeMatrix>(context, this);
}

NyLang::ClassID SymbolTypeMatrix::GetClassID() {
    return kCID;
}

SymbolTextureType::SymbolTextureType(Context& context, TextureType type, NyLang::SymbolTypeInfo *value, int dim)
        : SymbolType(0, context.New<TextureResourceType>(type, value->il, dim, false)), type(type), dim(dim), value(value) {

}

NyLang::ClassID SymbolTextureType::GetClassID() {
    return kCID;
}

NyLang::SymbolComponent * SymbolTextureType::Copy(Context& context) {
    return context.New<SymbolTextureType>(context, type, value, dim);
}

bool SymbolTextureType::Compare(NyLang::SymbolComponent *) {
    return false;
}

void SymbolTextureType::PrettyPrint(NyLang::PrettyStream &stream) {
    stream << "<texture type>";
}

NyLang::SymbolType * SymbolTextureType::PatternReplace(Context& context, std::list<NyLang::SymbolType *> &types) {
    // Only takes one argument
    if (types.empty()) {
        return nullptr;
    }
    NyLang::SymbolType * other = types.front();
    types.pop_front();

    if (other->GetClassID() != kCID) {
        return nullptr;
    }
    return NyLang::StaticCast<NyLang::SymbolType>(other->Copy(context));
}

bool SymbolTextureType::IsIncomplete() {
    return false;
}

SymbolSamplerType::SymbolSamplerType(Context& context) : SymbolType(0, context.New<SamplerResourceType>()) {

}

NyLang::ClassID SymbolSamplerType::GetClassID() {
    return kCID;
}

NyLang::SymbolComponent * SymbolSamplerType::Copy(Context& context) {
    return context.New<SymbolSamplerType>(context);
}

bool SymbolSamplerType::Compare(NyLang::SymbolComponent *) {
    return false;
}

void SymbolSamplerType::PrettyPrint(NyLang::PrettyStream &stream) {
    stream << "<sampler type>";
}

NyLang::SymbolType * SymbolSamplerType::PatternReplace(Context& context, std::list<NyLang::SymbolType *> &types) {
    // Only takes one argument
    if (types.empty()) {
        return nullptr;
    }
    NyLang::SymbolType * other = types.front();
    types.pop_front();

    if (other->GetClassID() != kCID) {
        return nullptr;
    }
    return NyLang::StaticCast<NyLang::SymbolType>(other->Copy(context));
}

bool SymbolSamplerType::IsIncomplete() {
    return false;
}
