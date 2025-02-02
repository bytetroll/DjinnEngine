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

    class NodeAuto : public Node {
    public:
        LANG_CLASSID("NodeAuto");

        NodeAuto(const Token &Tok, NodeBlock* Parameters, Node* Target);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Parameters
        NodeBlock* Parameters = nullptr;

        // Children
        std::list<NodeAuto*> children;

        // Supplied parameters
        bool isInstantiation = false;
        std::list<Symbol*> arguments;

        // Target Node
        Node* Target = nullptr;
    };
}