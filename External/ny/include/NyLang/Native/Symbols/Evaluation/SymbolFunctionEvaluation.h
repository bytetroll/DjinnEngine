#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//


#include <list>
#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    class SymbolFunction;

    /// General flow control
    struct SymbolFunctionEvaluation : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolFunctionEvaluation");

        /// Create component
        explicit SymbolFunctionEvaluation(SymbolFunction* function);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // ...
        SymbolFunction* function = nullptr;

        // Return symbol
        Symbol* returnSymbol = nullptr;

        // Argument symbols
        std::list<Symbol*> arguments;
    };
}