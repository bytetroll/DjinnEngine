#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <list>
#include <NyLang/Native/Symbols/SymbolType.h>
#include <NyLang/Frontend/Symbol/SymbolPair.h>

namespace NyLang {
    class SymbolVariable;
    struct SymbolNamed;

    // Auto Type
    struct SymbolTypeAuto : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeAuto");

        /// Create component
        explicit SymbolTypeAuto(Node* super, Symbol* target);

        /// Create component
        explicit SymbolTypeAuto(SymbolTypeAuto* other);

        /// Overrides
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        bool IsIncomplete() override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent* other) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Super node
        Node* super = nullptr;

        // Target
        Symbol* target = nullptr;

        // Parameters
        std::list<Symbol*> parameters;

        // Cached instantiations
        std::list<std::pair<SymbolPair<SymbolNamed>, std::list<Symbol*>>> cached;
    };
}