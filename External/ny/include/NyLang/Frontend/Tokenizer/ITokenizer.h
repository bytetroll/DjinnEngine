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