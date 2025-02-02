#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//


#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    namespace IL {
        class Member;
    }

    // Variable
    struct SymbolVariable : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolVariable");

        /// Create component
        explicit SymbolVariable(const String& name, Symbol* contained);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Name of this function
        String name;

        // Contained symbol
        Symbol* contained = nullptr;

        // Is this variable incomplete?
        bool isIncomplete = false;

        // Is this variable a member?
        bool isMember = false;

        // Member il
        IL::Member* il = nullptr;
    };
}