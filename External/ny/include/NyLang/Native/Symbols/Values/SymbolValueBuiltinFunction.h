#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>
#include <functional>

namespace NyLang {
    using BuiltinFunctionDelegate = std::function<Symbol*(const SymbolAnchor& anchor)>;

    // BuiltinFunction Value
    struct SymbolValueBuiltinFunction : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueBuiltinFunction");

        /// Create component
        explicit SymbolValueBuiltinFunction(const BuiltinFunctionDelegate& function);

        /// Create component
        explicit SymbolValueBuiltinFunction(SymbolValueBuiltinFunction* other);

        /// Overrides
        SymbolComponent *Copy(Context &context) override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        BuiltinFunctionDelegate function;
    };
}