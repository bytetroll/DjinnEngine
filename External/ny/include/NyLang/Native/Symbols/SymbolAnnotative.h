#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    /// Signifies that "this" symbol can be annotated
    struct SymbolAnnotative : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolAnnotative");

        /// Create component
        explicit SymbolAnnotative();

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;
    };
}