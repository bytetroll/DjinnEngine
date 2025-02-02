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

#include "IParser.h"

namespace NyLang {
    class Parser : public IParser {
    public:
        LANG_CLASSID("Lang.Parser");

        /// Construct
        /// \param registry
        Parser(Registry*registry);

        /// Overrides
        virtual NyLang::ParserContext* Parse(TokenizerContext*tokenizer) override;
        virtual NyLang::ParserContext* ParseExtern(TokenizerContext*tokenizer) override;
        virtual NyLang::ParserContext* ParseExpression(TokenizerContext*tokenizer) override;
        virtual ClassID GetClassID() override;
        virtual void AddSegmentHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserSegmentDelegate &delegate) override;
        virtual void AddPostSegmentHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserPostSegmentDelegate &delegate) override;
        virtual void AddDefinitionHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserDefinitionDelegate &delegate) override;
        virtual void AddSuccessorHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserSuccessorDelegate &delegate) override;
        virtual void AddBinaryHandler(ActionID aid, const Vector<ParserProperty> &properties, const ParserBinaryDelegate &delegate) override;
        virtual void AddDependency(const Dependency &dependency) override;
        virtual bool Configure() override;
        virtual void AddBinaryToken(TokenID id, int power) override;
        virtual int GetBinaryPower(TokenID id) override;
        virtual ParserEvent TryParseSegment(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent ParseSuccessor(ParserSnapshot &snapshot, ParserScopeStack scope, Node*lhs) override;
        virtual ParserEvent TryParseExpression(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent ParseStatementBlock(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent TryParseStatement(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent ParseDefinitionBlock(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent TryParseDefinition(ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent TrySegmentAction(ActionID aid, ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent TryPostSegmentAction(ActionID aid, ParserSnapshot &snapshot, ParserScopeStack scope, Node*lhs) override;
        virtual ParserEvent TryDefinitionAction(ActionID aid, ParserSnapshot &snapshot, ParserScopeStack scope) override;
        virtual ParserEvent TrySuccessorAction(ActionID aid, ParserSnapshot &snapshot, ParserScopeStack scope, Node*lhs) override;
        virtual ParserEvent TryBinaryAction(ActionID aid, ParserSnapshot &snapshot, ParserScopeStack scope, Token tok, Node*lhs, Node*rhs) override;

    protected:
        ParserEvent ParseBinaryRHS(ParserSnapshot &snapshot, ParserScopeStack scope, Node*lhs, int lastPrecedence = 1);

        template <typename T>
        struct Action {
            ActionID id;
            T delegate;
        };

    private:
        template <typename T>
        bool ResolveBucket(Vector<Action<T>>& bucket);

        Registry* registry = nullptr;

        // Trigger handlers
        UnorderedMap<TokenID::Type, Vector<Action<ParserSegmentDelegate>>> segmentTriggerHandlers;
        UnorderedMap<TokenID::Type, Vector<Action<ParserPostSegmentDelegate>>> postSegmentTriggerHandlers;
        UnorderedMap<TokenID::Type, Vector<Action<ParserDefinitionDelegate>>> definitionTriggerHandlers;
        UnorderedMap<TokenID::Type, Vector<Action<ParserSuccessorDelegate>>> successorTriggerHandlers;
        UnorderedMap<TokenID::Type, Vector<Action<ParserBinaryDelegate>>> binaryTriggerHandlers;

        // Handlers
        Vector<Action<ParserSegmentDelegate>> segmentHandlers;
        Vector<Action<ParserPostSegmentDelegate>> postSegmentHandlers;
        Vector<Action<ParserDefinitionDelegate>> definitionHandlers;
        Vector<Action<ParserSuccessorDelegate>> successorHandlers;
        Vector<Action<ParserBinaryDelegate>> binaryHandlers;

        // Mapped handlers
        UnorderedMap<ActionID::Type, ParserSegmentDelegate> segmentMap;
        UnorderedMap<ActionID::Type, ParserPostSegmentDelegate> postSegmentMap;
        UnorderedMap<ActionID::Type, ParserDefinitionDelegate> definitionMap;
        UnorderedMap<ActionID::Type, ParserSuccessorDelegate> successorMap;
        UnorderedMap<ActionID::Type, ParserBinaryDelegate> binaryMap;

        // Binary power mappings
        UnorderedMap<TokenID::Type, int> binaryPowers;

        // Dependencies
        Vector<Dependency> dependencies;
    };
}