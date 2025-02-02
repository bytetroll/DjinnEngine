//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/StdTypes.h>
#include "TokenizerSnapshot.h"
#include <functional>
#include <NyLang/Core/Host/Dependency.h>

namespace NyLang {
    class TokenizerContext;
    class Node;

    /// Tokenizer property
    struct TokenizerProperty {
        TokenizerProperty(const Dependency& dependency) : isDependency(true), dependency(dependency) {

        }

        TokenizerProperty(char ch) : isDependency(false), ch(ch) {

        }

        bool isDependency;

        Dependency dependency;
        char ch;
    };

    /// Tokenizer result
    enum class TokenizerResult {
        eFailed,
        eError,
        eOK
    };

    /// Delegate types
    using TokenizerDelegate = std::function<TokenizerResult(TokenizerSnapshot&)>;

    /// Tokenizer interface
    class ITokenizer : public IUnknown {
    public:
        LANG_INTERFACEID("Lang.ITokenizer");

        /// Tokenize from a string
        /// \param tokenizer
        /// \return
        virtual TokenizerContext* Tokenize(const char* str) = 0;

        /// Add a handler with given dependencies
        /// \param properties
        /// \param delegate
        virtual void AddHandler(ActionID aid, const Vector<TokenizerProperty>& properties, const TokenizerDelegate& delegate) = 0;

        /// Add a dependency
        /// \param dependency
        virtual void AddDependency(const Dependency& dependency) = 0;

        /// Resolves dependencies
        /// \return
        virtual bool Configure() = 0;
    };
}