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
    class NodeExpand : public Node {
    public:
        LANG_CLASSID("NodeExpand");

        NodeExpand(const Token &Tok, Node* L, Node* R);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // LHS
        Node* L = nullptr;

        // RHS
        Node* R = nullptr;
    };
}