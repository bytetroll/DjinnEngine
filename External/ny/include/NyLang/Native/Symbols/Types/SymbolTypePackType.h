#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>


namespace NyLang {
    namespace IL {
        class ClassType;
    }

    // Pack Type
    struct SymbolTypePackType : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypePackType");

        /// Create component
        explicit SymbolTypePackType(SymbolTypeInfo* TypeInfo, IL::ClassType* il = nullptr);

        /// Create component
        explicit SymbolTypePackType(SymbolTypePackType* other);

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