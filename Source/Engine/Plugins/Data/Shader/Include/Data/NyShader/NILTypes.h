//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <NyLang/Backend/IL/IType.h>

namespace Djinn::Data::NyShader {
    using namespace NyLang;
    
    class VectorType : public IL::IType {
    public:
        LANG_CLASSID("VectorType");

        /// Construct
        /// \param type
        /// \param x
        VectorType(IL::IType *type, unsigned x);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        IType* type = nullptr;
        unsigned x;
    };

    class MatrixType : public IL::IType {
    public:
        LANG_CLASSID("MatrixType");

        /// Construct
        /// \param type
        /// \param x
        /// \param y
        MatrixType(IL::IType *type, unsigned x, unsigned y);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        IType* type = nullptr;
        unsigned x;
        unsigned y;
    };

    enum class TextureType {
        eBuffer,
        eCube,
        eNone
    };

    class TextureResourceType : public IL::IType {
    public:
        LANG_CLASSID("TextureResourceType");

        /// Construct
        TextureResourceType(TextureType type, IL::IType *value, int dim, bool isShadowMap);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        TextureType type;
        int dim;
        bool isShadowMap;
        IL::IType* value = nullptr;
    };

    class TextureMapResourceType : public IL::IType {
    public:
        LANG_CLASSID("TextureMapResourceType");

        /// Construct
        TextureMapResourceType(TextureType type, IL::IType *value, int dim, bool isShadowMap, int count);

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;

        TextureType type;
        int dim;
        int count;
        bool isShadowMap;
        IL::IType* value = nullptr;
    };

    class SamplerResourceType : public IL::IType {
    public:
        LANG_CLASSID("SamplerResourceType");

        /// Construct
        SamplerResourceType();

        /// Overrides
        void PrettyPrint(PrettyStream &stream, IL::PrettyPrintContext& context) override;
        ClassID GetClassID() override;
    };
}