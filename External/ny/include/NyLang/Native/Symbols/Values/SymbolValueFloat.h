#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Float Value
    struct SymbolValueFloat : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueFloat");

        /// Create component
        explicit SymbolValueFloat(Context& context, float value);
        explicit SymbolValueFloat(Context& context, SymbolValueFloat* other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        float value;
    };
}