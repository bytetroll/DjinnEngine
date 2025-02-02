#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <vector>
#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeBlock;

    enum class IndirectionType {
        ePointer,
        ePack
    };

    class NodeType : public Node {
    public:
        LANG_CLASSID("NodeType");

        using IndirectionPair = std::pair<IndirectionType, Node*>;

        NodeType(const Token &Tok, Node*ID, const Vector<Token>& Specifiers, NodeBlock*NodeSpecifiers, NodeBlock*TmplParameters, const std::list<IndirectionPair>& IndirectionStack, NodeBlock* functionParams);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* ID = nullptr;

        // Specifiers
        Vector<Token> Specifiers;

        // Node Specifiers
        NodeBlock* NodeSpecifiers = nullptr;

        // Indirection Stack
        std::list<IndirectionPair> IndirectionStack;

        // Template parameters
        NodeBlock* TmplParameters = nullptr;

        // FunctionType parameters
        NodeBlock* functionParams = nullptr;
    };
}