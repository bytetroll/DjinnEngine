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
    class NodeBlock;

    class NodeScope : public Node {
    public:
        LANG_CLASSID("NodeScope");

        NodeScope(const Token &Tok, Node* id, Node* Body);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* id = nullptr;

        // Body
        Node* Body = nullptr;
    };
}