//
// Created by Dev on 10/29/2017.
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