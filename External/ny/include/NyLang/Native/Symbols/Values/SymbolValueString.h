#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // String Value
    struct SymbolValueString : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueString");

        /// Create component
        explicit SymbolValueString(Context& context, const String& value);

        /// Create component
        explicit SymbolValueString(Context& context, SymbolValueString* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        String value;
    };
}