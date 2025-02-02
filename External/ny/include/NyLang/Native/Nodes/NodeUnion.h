#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>
#include <NyLang/Core/Context.h>

namespace NyLang {
    class NodeUnion : public Node {
    public:
        LANG_CLASSID("NodeUnion");

        NodeUnion(const Token &Tok, Node* ID, Node* DefaultType, Node* Body);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* ID = nullptr;

        // Body
        Node* Body = nullptr;

        // Default Type
        // Can be void
        Node* DefaultType = nullptr;
    };
}