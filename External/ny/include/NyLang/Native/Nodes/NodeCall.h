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

    class NodeCall : public Node {
    public:
        LANG_CLASSID("NodeCall");

        NodeCall(const Token &Tok, Node* ID, NodeBlock* Arguments);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* ID = nullptr;

        // Arguments
        NodeBlock* Arguments = nullptr;
    };
}