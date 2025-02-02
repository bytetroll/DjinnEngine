#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Type Value
    struct SymbolValueType : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueType");

        /// Create component
        explicit SymbolValueType(Context& context, SymbolTypeInfo* value);

        /// Create component
        explicit SymbolValueType(Context& context, SymbolValueType* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // ...
        SymbolTypeInfo* type = nullptr;
    };
}