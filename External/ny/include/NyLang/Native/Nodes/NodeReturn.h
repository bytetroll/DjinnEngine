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

    class NodeReturn : public Node {
    public:
        LANG_CLASSID("NodeReturn");

        NodeReturn(const Token &Tok, Node* Value);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Value
        Node* Value = nullptr;
    };
}