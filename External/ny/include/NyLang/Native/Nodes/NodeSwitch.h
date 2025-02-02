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

    class NodeSwitch : public Node {
    public:
        LANG_CLASSID("NodeSwitch");

        NodeSwitch(const Token &Tok, Node* Target, Node* Cases);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Target
        Node* Target = nullptr;

        // Cases
        Node* Cases = nullptr;
    };
}