#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    // Placement
    struct SymbolPlacement : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolPlacement");

        /// Create component
        explicit SymbolPlacement(SymbolTree* Tree, const String& Name);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Name
        String Name;

        // Contained tree
        SymbolTree* Tree = nullptr;
    };
}