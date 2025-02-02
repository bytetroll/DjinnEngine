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
    class NodeDeclare : public Node {
    public:
        LANG_CLASSID("NodeDeclare");

        NodeDeclare(const Token &Tok, bool isExtern, Node*Type, Node*ID, bool isAutoBinding, Node*Binding);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Type
        Node* Type = nullptr;

        // ID
        Node* ID = nullptr;

        // Is extern
        bool isExtern;

        // Auto binding?
        bool isAutoBinding;

        // Binding
        Node* Binding = nullptr;
    };
}