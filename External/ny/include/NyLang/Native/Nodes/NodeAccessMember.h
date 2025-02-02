#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeAccessMember : public Node {
    public:
        LANG_CLASSID("NodeAccessMember");

        NodeAccessMember(const Token &Tok, Node* L, Node* R);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // LHS
        Node* L = nullptr;

        // RHS
        Node* R = nullptr;

        // ...
        SymbolTree* tree = nullptr;
    };
}