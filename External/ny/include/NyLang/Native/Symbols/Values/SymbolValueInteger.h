#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Integer Value
    struct SymbolValueInteger : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueInteger");

        /// Create component
        explicit SymbolValueInteger(Context& context, unsigned size, std::int64_t value);

        /// Create component
        explicit SymbolValueInteger(Context& context, SymbolValueInteger* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        unsigned size;
        std::int64_t value;
    };
}