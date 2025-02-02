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
    // Pack Value
    struct SymbolValuePack : public SymbolValue {
    public:
        LANG_CLASSID("SymbolValuePack");

        /// Create component
        explicit SymbolValuePack(Context& context, SymbolTypeInfo*typeInfo);

        /// Create component
        explicit SymbolValuePack(Context& context, SymbolValuePack *other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        /// Add a value
        /// \param value
        void AddValue(Symbol*value);

        /// Get values
        /// \return
        const Vector<Symbol*> &GetValues() const {
            return values;
        }

        // Contained type info
        SymbolTypeInfo* contained = nullptr;

    private:
        // Values
        Vector<Symbol*> values;
    };
}