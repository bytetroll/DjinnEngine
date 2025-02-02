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

    class NodeRoot : public Node {
    public:
        LANG_CLASSID("NodeRoot");

        NodeRoot(const Token& Tok, Node* target);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Target
        Node* target = nullptr;
    };
}