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
#include <functional>
#include <NyLang/Core/Host/Dependency.h>
#include <NyLang/Frontend/Token/Token.h>
#include "ParserSnapshot.h"
#include "ParserScope.h"

namespace NyLang {
    class ParserContext;
    class TokenizerContext;
    class Node;

    /// Parser property
    struct ParserProperty {
        ParserProperty(const Dependency& dependency) : isDependency(true), dependency(dependency) {

        }

        ParserProperty(TokenID tid) : isDependency(false), tid(tid) {

        }

        bool isDependency;

        Dependency dependency;
        TokenID tid;
    };

    /// Parser event
    struct ParserEvent {
        /// Construct as null
        ParserEvent(std::nullptr_t = nullptr) : isError(false) {

        }

        /// Construct from node
        /// \param node
        template<typename T = Node>
        ParserEvent(T*node) : isError(false), node(node) {

        }

        /// Construct error event
        /// \return
        static ParserEvent Error() {
            return ParserEvent(false);
        }

        /// Helper
        /// \return
        Node *operator->() const {
            return node;
        }

        /// Get node
        /// \return
        Node* Get() {
            return node;
        }

        /// Did this event succeed?
        /// \return
        operator bool() {
            return node && !isError;
        }

        /// Implicit node
        /// \return
        operator Node*() {
            return node;
        }

        /// Is this event an error?
        /// \return
        bool IsError() {
            return isError;
        }

    private:
        ParserEvent(bool) : isError(true) {

        }

        bool isError;
        Node* node = nullptr;
    };

    /// Delegate types
    using ParserSegmentDelegate = std::function<ParserEvent(ParserSnapshot &, ParserScopeStack)>;
    using ParserPostSegmentDelegate = std::function<ParserEvent(ParserSnapshot &, ParserScopeStack, Node* lhs)>;
    using ParserDefinitionDelegate = std::function<ParserEvent(ParserSnapshot &, ParserScopeStack)>;
    using ParserSuccessorDelegate = std::function<ParserEvent(ParserSnapshot &, ParserScopeStack, Node* lhs)>;
    using ParserBinaryDelegate = std::function<ParserEvent(ParserSnapshot &, ParserScopeStack, Token op, Node* lhs, Node* rhs)>;

    /// Parser interface
    class IParser : public IUnknown {
    public:
        LANG_INTERFACEID("Lang.IParser");

        /// Parse from a tokenizer context
        /// \param tokenizer
        /// \return
        virtual ParserContext* Parse(TokenizerContext*tokenizer) = 0;

        /// Parse from a tokenizer context
        /// \param tokenizer
        /// \return
        virtual ParserContext* ParseExtern(TokenizerContext*tokenizer) = 0;

        /// Parse from a tokenizer context
        /// \param tokenizer
        /// \return
        virtual ParserContext* ParseExpression(TokenizerContext*tokenizer) = 0;

        /// Try parse segment
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryParseSegment(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Try parse successor
        /// \param snapshot
        /// \param scope
        /// \param lhs
        /// \return
        virtual ParserEvent ParseSuccessor(ParserSnapshot &snapshot, ParserScopeStack scope, Node*lhs) = 0;

        /// Try parse expression
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryParseExpression(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Try parse statement block
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent ParseStatementBlock(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Try parse statement
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryParseStatement(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Try parse definition block
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent ParseDefinitionBlock(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Try parse definition
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryParseDefinition(ParserSnapshot &snapshot, ParserScopeStack scope) = 0;

        /// Add a segment handler with given properties
        /// \param aid
        /// \param properties
        /// \param delegate
        virtual void AddSegmentHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserSegmentDelegate &delegate) = 0;

        /// Add a post segment handler with given properties
        /// \param aid
        /// \param properties
        /// \param delegate
        virtual void AddPostSegmentHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserPostSegmentDelegate &delegate) = 0;

        /// Add a definition handler with given properties
        /// \param aid
        /// \param properties
        /// \param delegate
        virtual void AddDefinitionHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserDefinitionDelegate &delegate) = 0;

        /// Add a successor handler with given properties
        /// \param aid
        /// \param properties
        /// \param delegate
        virtual void AddSuccessorHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserSuccessorDelegate &delegate) = 0;

        /// Add a successor handler with given properties
        /// \param aid
        /// \param properties
        /// \param delegate
        virtual void AddBinaryHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserBinaryDelegate &delegate) = 0;

        /// Try segment by action id
        /// \param aid
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TrySegmentAction(ActionID aid, ParserSnapshot& snapshot, ParserScopeStack scope) = 0;

        /// Try post segment by action id
        /// \param aid
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryPostSegmentAction(ActionID aid, ParserSnapshot& snapshot, ParserScopeStack scope, Node* lhs) = 0;

        /// Try definition by action id
        /// \param aid
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryDefinitionAction(ActionID aid, ParserSnapshot& snapshot, ParserScopeStack scope) = 0;

        /// Try successor by action id
        /// \param aid
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TrySuccessorAction(ActionID aid, ParserSnapshot& snapshot, ParserScopeStack scope, Node* lhs) = 0;

        /// Try binary by action id
        /// \param aid
        /// \param snapshot
        /// \param scope
        /// \return
        virtual ParserEvent TryBinaryAction(ActionID aid, ParserSnapshot& snapshot, ParserScopeStack scope, Token tok, Node* lhs, Node* rhs) = 0;

        /// Add a binary token
        /// \param id
        /// \param power
        virtual void AddBinaryToken(TokenID id, int power) = 0;

        /// Get binary power
        /// \param id
        /// \return
        virtual int GetBinaryPower(TokenID id) = 0;

        /// Add a dependency
        /// \param dependency
        virtual void AddDependency(const Dependency& dependency) = 0;

        /// Resolves dependencies
        /// \return
        virtual bool Configure() = 0;
    };
}