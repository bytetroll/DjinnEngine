#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Double Value
    struct SymbolValueDouble : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueDouble");

        /// Create component
        explicit SymbolValueDouble(Context& context, double value);

        /// Create component
        explicit SymbolValueDouble(Context& context, SymbolValueDouble* other);

        /// Overrides
        SymbolComponent *Copy(Context &context) override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        double value;
    };
}