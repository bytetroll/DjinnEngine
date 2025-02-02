#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include "Meta.h"
#include <list>
#include <NyLang/Frontend/Symbol/Symbol.h>

namespace NyLang {
    class SymbolVariable;
    namespace IL {
        class Function;
    }

    // FunctionType
    struct SymbolFunction : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolFunction");

        /// Create component
        explicit SymbolFunction(Node* super);

        /// Create component
        explicit SymbolFunction(SymbolFunction* other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Name of this function
        String name;

        // Parent function
        SymbolFunction* Parent = nullptr;

        // Return Type
        Symbol* Return = nullptr;

        // Parameter Types
        std::list<Symbol*> Parameters;

        // Storage
        SymbolFunctionStorageSet Storage;

        // Decoration
        SymbolFunctionDecoration Decoration;

        // IL
        IL::Function* il = nullptr;

        // Is complete?
        bool isComplete = false;

        // Super
        Node* super = nullptr;
    };
}