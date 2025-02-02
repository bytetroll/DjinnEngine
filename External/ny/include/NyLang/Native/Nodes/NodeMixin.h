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
    class NodeMixin : public Node {
    public:
        LANG_CLASSID("NodeMixin");

        NodeMixin(const Token &Tok, Node* Mixin, bool isRuntimeScope);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;
        virtual void Clear() override;

        // Mixin
        Node* Mixin = nullptr;

        // Resulting node
        Node* Result = nullptr;

        // Is runtime scope
        bool isRuntimeScope;
    };
}