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
 on 11/2/2017.
//
#pragma once

#include <NyLang/Core/Shared.h>
#include <list>
#include <NyLang/Frontend/Token/Token.h>

namespace NyLang {
    class Node;
    class Symbol;
    struct SemanticContext;
    class DiagnosticBucket;
    namespace IL {
        class Block;
    }

    struct SymbolWrapper {
        Symbol* symbol = nullptr;
        Token tok;
    };

    class SemanticResolve {
    public:
        /// Apply the results of this resolve to another resolve
        /// \param node
        virtual bool Apply(SemanticResolve *resolve) = 0;

        /// Apply the results of this resolve to a node
        /// \param node
        virtual bool Apply(SemanticContext*context, Node *node) = 0;

        // Diagnostic bucket
        DiagnosticBucket* bucket = nullptr;

        // IL Block
        IL::Block* ilBlock = nullptr;
    };
}