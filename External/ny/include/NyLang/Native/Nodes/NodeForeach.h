#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeForeach : public Node {
    public:
        LANG_CLASSID("NodeForeach");

        NodeForeach(const Token &Tok, Node*Var, Node*Container, Node*Body);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Header
        Node* Var = nullptr;

        // Condition
        Node* Container = nullptr;

        // Body
        Node* Body = nullptr;
    };
}