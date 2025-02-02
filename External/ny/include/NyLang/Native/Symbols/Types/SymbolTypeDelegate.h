#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <list>
#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    class SymbolFunction;

    // Delegate Type
    struct SymbolTypeDelegate : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeDelegate");

        /// Create component
        explicit SymbolTypeDelegate(Context& context, SymbolTypeInfo*returnType, const std::list<SymbolTypeInfo*> &parameterTypes);

        /// Create component
        explicit SymbolTypeDelegate(Context& context, SymbolFunction*function);

        /// Create component
        explicit SymbolTypeDelegate(Context& context, SymbolTypeDelegate* other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        bool IsIncomplete() override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Return type
        SymbolTypeInfo* returnType = nullptr;

        // Parameter types
        std::list<SymbolTypeInfo*> parameterTypes;
    };
}