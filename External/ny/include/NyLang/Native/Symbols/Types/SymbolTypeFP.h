#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    // Floating Point Type
    struct SymbolTypeFP : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeFP");

        /// Create component
        explicit SymbolTypeFP(Context& context, unsigned BitWidth = 4 * 8);

        /// Create component
        explicit SymbolTypeFP(SymbolTypeFP* other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        bool IsIncomplete() override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;
    };
}