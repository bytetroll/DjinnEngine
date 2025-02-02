#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeIf : public Node {
    public:
        LANG_CLASSID("NodeIf");

        NodeIf(const Token &Tok, Node* Condition, Node* Then, Node* Else);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Condition
        Node* Condition = nullptr;

        // Then
        Node* Then = nullptr;

        // Else
        Node* Else = nullptr;

        bool cached = false;
    };
}