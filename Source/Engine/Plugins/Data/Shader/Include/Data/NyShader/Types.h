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

#include <NyLang/Native/Symbols/SymbolType.h>
#include "NILTypes.h"

namespace Djinn::Data::NyShader {
    using namespace NyLang;
    
    struct SymbolTypeVector : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeVector");

        /// Construct
        /// \param TypeInfo
        /// \param CountX
        explicit SymbolTypeVector(Context& context, SymbolTypeInfo *TypeInfo, unsigned CountX);

        /// Construct
        /// \param other
        explicit SymbolTypeVector(Context& context, SymbolTypeVector *other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        bool Compare(SymbolComponent *other) override;
        SymbolType * PatternReplace(Context& context, std::list<SymbolType *> &types) override;
        bool IsIncomplete() override;
        SymbolComponent * Copy(Context& context) override;
        ClassID GetClassID() override;

        // Contained TypeInfo
        SymbolTypeInfo * ContainedTypeInfo;

        // Size of vector
        unsigned CountX;
    };

    struct SymbolTypeMatrix : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeMatrix");

        /// Construct
        /// \param TypeInfo
        /// \param CountX
        /// \param CountY
        explicit SymbolTypeMatrix(Context& context, SymbolTypeInfo *TypeInfo, unsigned CountX, unsigned CountY);

        /// Construct
        /// \param other
        explicit SymbolTypeMatrix(Context& context, SymbolTypeMatrix *other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        bool Compare(SymbolComponent *other) override;
        SymbolType * PatternReplace(Context& context, std::list<SymbolType *> &types) override;
        bool IsIncomplete() override;
        SymbolComponent * Copy(Context& context) override;
        ClassID GetClassID() override;

        // Contained TypeInfo
        SymbolTypeInfo * ContainedTypeInfo;

        // Size of vector
        unsigned CountX;
        unsigned CountY;
    };

    class SymbolTextureType : public SymbolType {
    public:
        LANG_CLASSID("SymbolTextureType");

        /// Construct
        /// \param type
        /// \param value
        /// \param dim
        explicit SymbolTextureType(Context& context, TextureType type, SymbolTypeInfo *value, int dim);

        /// Overrides
        ClassID GetClassID() override;
        SymbolComponent * Copy(Context& context) override;
        bool Compare(SymbolComponent *other) override;
        void PrettyPrint(PrettyStream &stream) override;
        SymbolType * PatternReplace(Context& context, std::list<SymbolType *> &types) override;
        bool IsIncomplete() override;

        TextureType type;
        int dim;
        SymbolTypeInfo * value;
    };

    class SymbolSamplerType : public SymbolType {
    public:
        LANG_CLASSID("SymbolSamplerType");

        /// Construct
        /// \param type
        /// \param value
        /// \param dim
        explicit SymbolSamplerType(Context& context);

        /// Overrides
        ClassID GetClassID() override;
        SymbolComponent * Copy(Context& context) override;
        bool Compare(SymbolComponent *other) override;
        void PrettyPrint(PrettyStream &stream) override;
        SymbolType * PatternReplace(Context& context, std::list<SymbolType *> &types) override;
        bool IsIncomplete() override;
    };
}