#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    class SymbolVariable;

    // Placeholder Type
    struct SymbolTypePlaceholder : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypePlaceholder");

        /// Create component
        explicit SymbolTypePlaceholder(SymbolVariable* variable, SymbolTypeInfo* contained);

        /// Create component
        explicit SymbolTypePlaceholder(const SymbolTypePlaceholder& other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool IsIncomplete() override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Contained Symbol
        SymbolVariable* variable = nullptr;
        SymbolTypeInfo* contained = nullptr;
    };
}