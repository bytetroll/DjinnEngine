#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    // ScopedBlock
    struct SymbolScopedBlock : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolScopedBlock");

        /// Create component
        explicit SymbolScopedBlock();

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Resulting symbol
        Symbol* result = nullptr;
    };
}