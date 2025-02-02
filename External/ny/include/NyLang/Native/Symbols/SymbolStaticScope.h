#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    class SemanticScopeStack;

    // StaticScope
    struct SymbolStaticScope : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolStaticScope");

        /// Create component
        explicit SymbolStaticScope(Node* node);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Node
        Node* node = nullptr;

        // Scopes
        SemanticScopeStack* scopes = nullptr;
    };
}