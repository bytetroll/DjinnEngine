#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeBlock;

    class NodeObject : public Node {
    public:
        LANG_CLASSID("NodeObject");

        using Storage = std::pair<Token, Node*>;

        NodeObject(const Token &Tok, const Token& type, Node* ID, NodeBlock* Bases, Node* Body, const std::list<Storage>& storage);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Type token
        Token type;

        // ID
        Node* ID = nullptr;

        // Bases
        NodeBlock* Bases = nullptr;

        // Body
        Node* Body = nullptr;

        // Storage data
        std::list<Storage> storage;
    };
}