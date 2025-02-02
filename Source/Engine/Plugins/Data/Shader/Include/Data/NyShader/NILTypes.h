//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/29/2017.
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