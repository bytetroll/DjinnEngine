#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    // Array Type
    struct SymbolTypeArray : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeArray");

        /// Create component
        explicit SymbolTypeArray(Context& context, SymbolTypeInfo* TypeInfo, unsigned Count = 0);

        /// Create component
        explicit SymbolTypeArray(SymbolTypeArray* other);

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
        // Can be 0 (Variable)
        unsigned Count;
    };
}