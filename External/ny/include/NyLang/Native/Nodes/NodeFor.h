#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeFor : public Node {
    public:
        LANG_CLASSID("NodeFor");

        NodeFor(const Token &Tok, Node*Header, Node*Condition, Node*Step, Node*Body);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Header
        Node* Header = nullptr;

        // Condition
        Node* Condition = nullptr;

        // Step
        Node* Step = nullptr;

        // Body
        Node* Body = nullptr;
    };
}