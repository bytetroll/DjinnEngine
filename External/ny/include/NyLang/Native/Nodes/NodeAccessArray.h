#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    class NodeAccessArray : public Node {
    public:
        LANG_CLASSID("NodeAccessArray");

        /// Construct
        /// \param Tok
        /// \param L
        /// \param Index
        NodeAccessArray(const Token &Tok, Node*L, Node*Index);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // LHS
        Node* L = nullptr;

        // Index
        Node* Index = nullptr;
    };
}