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

    class NodeMemberInitializerList : public Node {
    public:
        LANG_CLASSID("NodeMemberInitializerList");

        NodeMemberInitializerList(const Token &Tok, Node* Target, NodeBlock* Values);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Target
        Node* Target = nullptr;

        // Values
        NodeBlock* Values = nullptr;
    };
}