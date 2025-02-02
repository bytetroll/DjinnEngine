#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>

namespace NyLang {
    class SymbolObject;

    // Object Type
    struct SymbolTypeObject : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypeObject");

        /// Create component
        explicit SymbolTypeObject(Context& context, SymbolObject* Object);

        /// Create component
        explicit SymbolTypeObject(SymbolTypeObject* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool IsIncomplete() override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        // Object
        SymbolObject* Object = nullptr;
    };
}