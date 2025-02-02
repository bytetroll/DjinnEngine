#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/Meta.h>
#include <NyLang/Frontend/Node/Node.h>
#include <NyLang/Core/Context.h>

namespace NyLang {
    class NodeBlock;

    class NodeFunction : public Node {
    public:
        LANG_CLASSID("NodeFunction");

        NodeFunction(const Token &Tok, bool isExtern, NodeBlock* Specifiers, Node*ReturnType, Node*ID, NodeBlock*Parameters, Node*Body, const SymbolFunctionDecoration& decoration, const Token& op);

        /// Overrides
        Node*TemplatedCopy(Context &context) override;
        virtual ClassID GetClassID() override;

        // Return Type
        Node* ReturnType = nullptr;

        // ID
        Node* ID = nullptr;

        // Parameters
        NodeBlock* Parameters = nullptr;

        // Body
        Node* Body = nullptr;

        // Specifiers
        NodeBlock* Specifiers = nullptr;

        // Decoration
        SymbolFunctionDecoration decoration;

        // Is extern
        bool isExtern;

        // Special op
        Token op;
    };
}