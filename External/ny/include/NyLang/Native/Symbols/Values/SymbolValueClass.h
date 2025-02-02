#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    class SymbolObject;

    // Class Value
    struct SymbolValueClass : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueClass");

        /// Create component
        explicit SymbolValueClass(Context& context, SymbolObject* obj);

        /// Create component
        explicit SymbolValueClass(Context& context, SymbolValueClass* other);

        /// Overrides
        SymbolComponent *Copy(Context &context) override;
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;
        virtual IL::ValueAddressPair &GetIL(Context &context) override;

        /// Get value
        /// \param sym
        /// \return
        SymbolValueInfo* GetValue(Context& context, Symbol* sym);

        // Object
        SymbolObject* object = nullptr;

        // Value map
        UnorderedMap<Symbol*, SymbolValueInfo*> values;
    };
}