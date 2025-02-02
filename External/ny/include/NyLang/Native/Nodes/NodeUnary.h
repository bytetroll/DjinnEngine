#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Node/Node.h>

namespace NyLang {
    enum class NodeUnaryPrecendance {
        LEFT,
        RIGHT
    };

    class NodeUnary : public Node {
    public:
        LANG_CLASSID("NodeUnary");

        NodeUnary(const Token &Tok, NodeUnaryPrecendance Precendance, Node* Target);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Precendance
        NodeUnaryPrecendance Precendance;

        // Target
        Node* Target = nullptr;
    };
}