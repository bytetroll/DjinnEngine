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
 on 10/29/2017.
//
#pragma once

#include "IMessage.h"

namespace NyLang {
#define LANG_SIMPLEMESSAGE(NAME, SEVERITY, MESSAGE) \
    class NAME : public NyLang::IMessage { \
    public: \
        NAME(const NyLang::Token &tok) : tok(tok) {} \
        \
        virtual void PrettyPrint(NyLang::PrettyStream &stream, NyLang::DiagnosticChain& chain) override { \
            MESSAGE; \
        } \
        virtual NyLang::TokenLoc GetLocation() override { \
            return tok.Loc; \
        } \
        virtual NyLang::MessageSeverity GetSeverity() override { \
            return SEVERITY;\
        } \
        \
    private: \
        NyLang::Token tok; \
    };

#define LANG_SIMPLEMESSAGE_1ARG(NAME, SEVERITY, A, a, MESSAGE) \
    class NAME : public NyLang::IMessage { \
    public: \
        NAME(const NyLang::Token &tok, A a) : tok(tok), a(a) {} \
        \
        virtual void PrettyPrint(NyLang::PrettyStream &stream, NyLang::DiagnosticChain& chain) override { \
            MESSAGE; \
        } \
        virtual NyLang::TokenLoc GetLocation() override { \
            return tok.Loc; \
        } \
        virtual NyLang::MessageSeverity GetSeverity() override { \
            return SEVERITY;\
        } \
        \
    private: \
        A a; \
        NyLang::Token tok; \
    };

#define LANG_SIMPLEMESSAGE_2ARG(NAME, SEVERITY, A, a, B, b, MESSAGE) \
    class NAME : public NyLang::IMessage { \
    public: \
        NAME(const NyLang::Token &tok, A a, B b) : tok(tok), a(a), b(b) {} \
        \
        virtual void PrettyPrint(NyLang::PrettyStream &stream, NyLang::DiagnosticChain& chain) override { \
            MESSAGE; \
        } \
        virtual NyLang::TokenLoc GetLocation() override { \
            return tok.Loc; \
        } \
        virtual NyLang::MessageSeverity GetSeverity() override { \
            return SEVERITY;\
        } \
        \
    private: \
        A a; \
        B b; \
        NyLang::Token tok; \
    };

#define LANG_SIMPLEMESSAGE_3ARG(NAME, SEVERITY, A, a, B, b, C, c, MESSAGE) \
    class NAME : public NyLang::IMessage { \
    public: \
        NAME(const NyLang::Token &tok, const A& a, const B& b, const C& c) : tok(tok), a(a), b(b), c(c) {} \
        \
        virtual void PrettyPrint(NyLang::PrettyStream &stream, NyLang::DiagnosticChain& chain) override { \
            MESSAGE; \
        } \
        virtual NyLang::TokenLoc GetLocation() override { \
            return tok.Loc; \
        } \
        virtual NyLang::MessageSeverity GetSeverity() override { \
            return SEVERITY;\
        } \
        \
    private: \
        A a; \
        B b; \
        C c; \
        Token tok; \
    };

    /// Simple passthrough message
    class MessageString : public IMessage {
    public:
        MessageString(TokenLoc loc, MessageSeverity severity, const String& str) : loc(loc), severity(severity), str(str) {
            // ...
        }

        MessageString(MessageSeverity severity, const String& str) : severity(severity), str(str) {
            loc.Line = 0;
        }

        virtual void PrettyPrint(PrettyStream &stream, DiagnosticChain& chain) override {
            stream << str;
        }

        virtual TokenLoc GetLocation() override {
            return loc;
        }

        virtual MessageSeverity GetSeverity() override {
            return severity;
        }

    private:
        TokenLoc loc;
        MessageSeverity severity;
        String str;
    };
}