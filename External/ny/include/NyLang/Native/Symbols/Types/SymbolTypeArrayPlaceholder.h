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

    // Array Type
    struct SymbolTypeArrayPlaceholder : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeArrayPlaceholder");

        /// Create component
        explicit SymbolTypeArrayPlaceholder(SymbolTypeInfo* TypeInfo, SymbolVariable* count);

        /// Create component
        explicit SymbolTypeArrayPlaceholder(SymbolTypeArrayPlaceholder* other);

        /// Overrides
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        bool IsIncomplete() override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent* other) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Contained TypeInfo
        SymbolTypeInfo* ContainedTypeInfo = nullptr;

        // Size of vector
        SymbolVariable* count = nullptr;
    };
}