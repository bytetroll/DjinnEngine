#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    class SymbolFunction;

    // Delegate Value
    struct SymbolValueDelegate : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueDelegate");

        /// Create component
        explicit SymbolValueDelegate(Context& context, SymbolFunction* function);

        /// Create component
        explicit SymbolValueDelegate(Context& context, SymbolValueDelegate* other);

        /// Overrides
        SymbolComponent *Copy(Context &context) override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // ...
        SymbolFunction* function = nullptr;
    };
}