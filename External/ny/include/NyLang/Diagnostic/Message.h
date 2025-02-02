//
// Created by Dev on 10/29/2017.
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