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

    class NodeAnnotation : public Node {
    public:
        LANG_CLASSID("NodeAnnotation");

        NodeAnnotation(const Token &Tok, NodeBlock* Attributes, Node* Target);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Attributes
        NodeBlock* Attributes = nullptr;

        // Target Node
        Node* Target = nullptr;
    };
}