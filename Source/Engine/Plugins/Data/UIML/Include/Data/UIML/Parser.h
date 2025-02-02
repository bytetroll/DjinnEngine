//
// Created by Dev on 5/5/2018.
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
