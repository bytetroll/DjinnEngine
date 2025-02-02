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
#include <Core/String.h>
#include <Core/Pair.h>
#include "Token.h"
#include "AST.h"

namespace Djinn::Memory {
    class Context;
}

namespace Djinn::Data::UIML {
    class Diagnostic;
    struct TokenBlob;
    struct ParserBlob;

    class Parser {
    public:
        /// Initialize this tokenizer
        /// \param context
        /// \return
        COM::Result Initialize(Memory::Context *context, Diagnostic *diag);

        /// Compile from a source
        /// \param source
        /// \param out
        /// \return
        COM::Result Compile(TokenBlob *blob, ParserBlob *out);

    protected:
        struct Snapshot {
            bool Is(const TokenID &hid) {
                return it->type == hid;
            }

            bool PeekIs(const TokenID &hid, UInt32 count = 1) {
                if (it + count > end) {
                    return false;
                }
                return (it + count)->type == hid;
            }

            bool Next() {
                return !(it == end || (++it) == end);
            }

            bool Next(UInt32 count) {
                if (it == end) {
                    return false;
                }
                if ((it += count) >= end) {
                    it = end;
                    return true;
                }
                return false;
            }

            bool IsEnd() {
                return it == end;
            }

            Token Tok() {
                return *it;
            }

            TokenID TokID() {
                return it->type;
            }

            Span TokSpan() {
                return it->span;
            }

            Token *it = nullptr;
            Token *end = nullptr;
        };

        COM::Result ParseBody(Snapshot &snapshot, Node **out);

        COM::Result ParseStyleBody(Snapshot &snapshot, Node **out);

        COM::Result ParseElement(Snapshot &snapshot, Node **out);

        COM::Result ParseStyleElement(Snapshot &snapshot, Node **out);

        COM::Result ParseValue(Snapshot &snapshot, Node **out);

        COM::Result ParseHeaders(Snapshot &snapshot, Core::StackArray4<StyleBind> &out);

        COM::Result ParseHeader(Snapshot &snapshot, StyleBind &out);

    private:
        Memory::Context *context;
        Diagnostic *diag;
    };
}
