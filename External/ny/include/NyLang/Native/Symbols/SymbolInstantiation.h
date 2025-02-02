#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>
#include <list>

namespace NyLang {
    struct SymbolNamed;

    // Instantiation
    struct SymbolInstantiation : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolInstantiation");

        /// Create component
        explicit SymbolInstantiation(SymbolNamed* original, const std::list<Symbol*>& arguments);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Original symbol
        SymbolNamed* original = nullptr;

        // Arguments
        std::list<Symbol*> arguments;
    };
}