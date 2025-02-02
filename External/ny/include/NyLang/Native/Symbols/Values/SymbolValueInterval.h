#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Interval Value
    struct SymbolValueInterval : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueInterval");

        /// Create component
        explicit SymbolValueInterval(SymbolValueInfo* min, SymbolValueInfo* max);

        /// Create component
        explicit SymbolValueInterval(Context& context, SymbolValueInterval* other);

        /// Overrides
        SymbolComponent *Copy(Context &context) override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Range
        SymbolValueInfo* min = nullptr;
        SymbolValueInfo* max = nullptr;
    };
}