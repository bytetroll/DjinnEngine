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
    class NodeOption : public Node {
    public:
        LANG_CLASSID("NodeOption");

        NodeOption(const Token &Tok, Node* ID, Node* Type, Node* Value);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // ID
        Node* ID = nullptr;

        // Type
        Node* Type = nullptr;

        // Value
        Node* Value = nullptr;
    };
}