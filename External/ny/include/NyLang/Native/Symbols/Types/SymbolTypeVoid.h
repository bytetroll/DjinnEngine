#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    // Void Type
    struct SymbolTypeVoid : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeVoid");

        /// Create component
        explicit SymbolTypeVoid(Context& context);

        /// Create component
        explicit SymbolTypeVoid(Context& context, SymbolTypeVoid* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        bool IsIncomplete() override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;
    };
}