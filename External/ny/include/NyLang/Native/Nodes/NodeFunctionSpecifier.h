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
    class NodeFunctionSpecifier : public Node {
    public:
        LANG_CLASSID("NodeFunctionSpecifier");

        NodeFunctionSpecifier(const Token& Tok, Node* Arguments);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Arguments
        Node* Arguments = nullptr;
    };
}