#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    // Pointer Type
    struct SymbolTypePointer : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypePointer");

        /// Create component
        explicit SymbolTypePointer(Context& context, SymbolTypeInfo* TypeInfo);

        /// Create component
        explicit SymbolTypePointer(SymbolTypePointer* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        bool IsIncomplete() override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Contained TypeInfo
        SymbolTypeInfo* ContainedTypeInfo = nullptr;
    };
}