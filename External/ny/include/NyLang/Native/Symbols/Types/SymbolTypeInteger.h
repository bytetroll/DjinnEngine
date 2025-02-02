#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    // Integer Type
    struct SymbolTypeInteger : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeInteger");

        /// Create component
        explicit SymbolTypeInteger(Context& context, bool IsUnsigned, unsigned BitWidth = 4 * 8);

        /// Create component
        explicit SymbolTypeInteger(Context& context, SymbolTypeInteger* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool IsIncomplete() override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Is this integer type unsigned?
        bool IsUnsigned;
    };
}