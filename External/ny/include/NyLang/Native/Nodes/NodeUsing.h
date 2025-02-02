#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeUsing : public Node {
    public:
        LANG_CLASSID("NodeUsing");

        NodeUsing(const Token &Tok, Node* ID, Node* Target);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* ID = nullptr;

        // Target
        Node* Target = nullptr;
    };
}