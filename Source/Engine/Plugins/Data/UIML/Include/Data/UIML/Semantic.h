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
 on 5/5/2018.
//
#pragma once

#include <COM/Result.h>
#include "SemanticMode.h"
#include "AST.h"
#include "SemanticScope.h"

namespace Djinn::Memory {
    class Context;
}

namespace Djinn::Data::UIML {
    class Diagnostic;
    struct ParserBlob;
    struct SemanticBlob;

    class Semantic {
    public:
        /// Initialize this semantic verifier
        /// \param context
        /// \return
        COM::Result Initialize(Memory::Context *context, Diagnostic *diag);

        /// Compile from a parsed
        /// \param source
        /// \param out
        /// \return
        COM::Result Compile(ParserBlob *blob, SemanticBlob *out);

    protected:
        COM::Result Visit(Node* node, SemanticScope scope);
        COM::Result Visit(BodyNode* node, SemanticScope scope);
        COM::Result Visit(StyleNode* node, SemanticScope scope);
        COM::Result Visit(ElementNode* node, SemanticScope scope);
        COM::Result Visit(StyleElementNode* node, SemanticScope scope);
        COM::Result Visit(StyleAttributeNode* node, SemanticScope scope);
        COM::Result Visit(StyleUsingNode* node, SemanticScope scope);
        COM::Result Visit(ValueNode* node, SemanticScope scope);
        COM::Result Visit(RGBANode* node, SemanticScope scope);
        COM::Result Visit(ArrayNode* node, SemanticScope scope);
        COM::Result Visit(ComponentNode* node, SemanticScope scope);
        COM::Result Visit(GroupNode* node, SemanticScope scope);
        COM::Result Visit(CategoryNode* node, SemanticScope scope);
        COM::Result Visit(BindNode* node, SemanticScope scope);
        COM::Result Visit(DataNode* node, SemanticScope scope);

    private:
        Memory::Context *context = nullptr;
        Diagnostic *diag = nullptr;
        SemanticBlob* out = nullptr;
    };
}
