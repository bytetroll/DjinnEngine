#include <Data/NyShader/NILTypes.h>
#include <Core/Common.h>

DJINN_NS3(Data, NyShader);

VectorType::VectorType(NyLang::IL::IType *type, unsigned x) : NyLang::IL::IType(x * type->GetBitWidth()), type(type), x(x) {

}

NyLang::ClassID VectorType::GetClassID() {
    return kCID;
}

void VectorType::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    type->PrettyPrint(stream, context);
    stream << "_" << x;
}

MatrixType::MatrixType(NyLang::IL::IType *type, unsigned x, unsigned y) : NyLang::IL::IType(x * y * type->GetBitWidth()), type(type), x(x), y(y) {

}

NyLang::ClassID MatrixType::GetClassID() {
    return kCID;
}

void MatrixType::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &context) {
    type->PrettyPrint(stream, context);
    stream << "_" << x << "x" << y;
}

TextureResourceType::TextureResourceType(TextureType type, IL::IType *value, int dim, bool isShadowMap) : NyLang::IL::IType(0), type(type), value(value), dim(dim), isShadowMap(isShadowMap) {

}

void TextureResourceType::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &) {
    stream << "texture";
}

NyLang::ClassID TextureResourceType::GetClassID() {
    return kCID;
}

SamplerResourceType::SamplerResourceType() : NyLang::IL::IType(0) {

}

void SamplerResourceType::PrettyPrint(NyLang::PrettyStream &stream, NyLang::IL::PrettyPrintContext &) {
    stream << "sampler";
}

NyLang::ClassID SamplerResourceType::GetClassID() {
    return kCID;
}

TextureMapResourceType::TextureMapResourceType(TextureType type, IL::IType *value, int dim, bool isShadowMap, int count) : NyLang::IL::IType(0), type(type), value(value), dim(dim), isShadowMap(isShadowMap), count(count) {

}

void TextureMapResourceType::PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext &context) {
    stream << "texturemap";
}

ClassID TextureMapResourceType::GetClassID() {
    return kCID;
}
