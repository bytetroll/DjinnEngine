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
#include <NyLang/Core/Exception.h>
#include <functional>
#include "SemanticScopeStack.h"

namespace NyLang {
    struct SemanticContext;
    class ParserContext;
    class Node;

    /// Semantic mode
    enum class SemanticMode {
        eDiscover = 1,
        eResolve = (int)eDiscover << 1,
        eEvaluate = (int)eResolve << 1
    };

    /// Common exceptions
    LANG_EXCEPTION(SemanticNoHandler);
    LANG_EXCEPTION(SemanticNotImplemented);
    LANG_EXCEPTION(SemanticUnexpectedNode);
    LANG_EXCEPTION(SemanticUnexpectedSymbol);
    LANG_EXCEPTION(SemanticUnexpectedToken);

    /// Semantic Result Types
    enum class SemanticResult {
        eOK,
        eError,
        eEarlyOut
    };

    /// Semantic event
    struct SemanticEvent {
        SemanticEvent(SemanticResult result = SemanticResult::eOK) : result(result) {

        }

        SemanticEvent(bool result) : result(result ? SemanticResult::eOK : SemanticResult::eError) {

        }

        operator bool() {
            return result == SemanticResult::eOK;
        }

        SemanticResult result;
    };

    /// Delegate types
    using SemanticDelegate = std::function<SemanticEvent(SemanticContext*, Node *, const SemanticScopeStack&, SemanticMode)>;

    /// Semantic interface
    class ISemantic : public IUnknown {
    public:
        LANG_INTERFACEID("Lang.ISemantic");

        /// Generate from parser context
        /// \param parser
        /// \return
        virtual SemanticContext* Generate(ParserContext*parser) = 0;

        /// Visit node with all modes
        /// \param node
        /// \param mode
        /// \return
        virtual SemanticEvent Visit(SemanticContext*context, Node *node, SemanticScopeStack scopes) = 0;

        /// Visit node
        /// \param node
        /// \param mode
        /// \return
        virtual SemanticEvent Visit(SemanticContext*context, Node *node, SemanticScopeStack scopes, SemanticMode mode) = 0;

        /// Add a handler with given properties
        /// \param aid
        /// \param triggers
        /// \param delegate
        virtual void AddHandler(ActionID aid, const Vector<ClassID> &triggers, const SemanticDelegate &delegate) = 0;
    };
}