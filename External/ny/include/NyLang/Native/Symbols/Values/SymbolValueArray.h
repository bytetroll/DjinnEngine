#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <list>
#include <vector>
#include <NyLang/Native/Symbols/SymbolValue.h>

namespace NyLang {
    // Array Value
    struct SymbolValueArray : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValueArray");

        /// Create component
        explicit SymbolValueArray(Context& context, SymbolTypeInfo* typeInfo);

        /// Create component
        explicit SymbolValueArray(Context& context, SymbolValueArray* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        /// Add a value
        /// \param value
        void AddValue(Context& context, SymbolValueInfo* value);

        /// Get values
        /// \return
        const Vector<SymbolValueInfo*>& GetValues() const {
            return values;
        }

        // Contained type info
        SymbolTypeInfo* contained = nullptr;

    private:
        // Values
        Vector<SymbolValueInfo*> values;
    };
}