#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeTypeSpecifier : public Node {
    public:
        LANG_CLASSID("NodeTypeSpecifier");

        NodeTypeSpecifier(const Token& Tok, Node* ScopedType);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ScopedType
        Node* ScopedType = nullptr;
    };
}