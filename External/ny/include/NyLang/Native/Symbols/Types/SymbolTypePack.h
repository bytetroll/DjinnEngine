#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Native/Symbols/SymbolType.h>
#include <list>

namespace NyLang {
    namespace IL {
        class ClassType;
    }

    // PackInstance Type
    struct SymbolTypePack : public SymbolType {
    public:
        LANG_CLASSID("SymbolTypePack");

        /// Create component
        explicit SymbolTypePack(SymbolTypeInfo* type, IL::ClassType* il = nullptr);

        /// Create component
        explicit SymbolTypePack(SymbolTypePack* other);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        bool Compare(SymbolComponent* other) override;
        SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) override;
        bool IsIncomplete() override;
        SymbolComponent* Copy(Context& context) override;
        virtual ClassID GetClassID() override;

        /// Add type
        /// \param type
        void AddType(SymbolTypeInfo* type);

        /// Get types of this pack
        /// \return
        const auto& GetTypes() const {
            return types;
        }

        // Original type
        SymbolTypeInfo* type = nullptr;

    private:
        // ...
        std::list<SymbolTypeInfo*> types;
    };
}