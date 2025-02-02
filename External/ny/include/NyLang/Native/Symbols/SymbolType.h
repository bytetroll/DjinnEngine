#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/BitSet.h>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include "Meta.h"
#include <list>

namespace NyLang {
    namespace IL {
        class IType;
    }

    /// Represents a completed type, comparable
    struct SymbolType : public SymbolComponent {
    public:
        /// Perform a pattern replacement of this type, does not have to perform replacement on all given types
        /// \param types
        /// \return - Null on invalid replacement
        virtual SymbolType *PatternReplace(Context &context, std::list<NyLang::SymbolType *> &types) = 0;

        /// Does this type rely on information currently not available?
        /// \return
        virtual bool IsIncomplete() = 0;

        /// Get bit width, may not be available
        /// \return - Returns 0 on unavailable
        unsigned GetBitWidth() const {
            return BitWidth;
        }

        // IL
        IL::IType* il = nullptr;

    protected:
        /// Create component
        explicit SymbolType(unsigned BitWidth, IL::IType* il);

        // Width of this type
        // May not be available (0)
        unsigned BitWidth;
    };

    // Represents a usable type
    // Needed to seperate logic
    struct SymbolTypeInfo : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolTypeInfo");

        /// Create component
        explicit SymbolTypeInfo(Context& context, SymbolType* Type, const SymbolTypeStorageSet& Storage = SymbolTypeStorageSet());

        /// Create component
        explicit SymbolTypeInfo(Context& context, SymbolTypeInfo* Info);

        /// Perform a pattern replacement of this type
        /// \param type
        /// \return - Null on invalid replacement
        SymbolTypeInfo* PatternReplace(Context& context, std::list<NyLang::SymbolTypeInfo*> &types);

        /// Does this type rely on information currently not available?
        /// \return
        bool IsIncomplete();

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        /// Set storage information
        /// \param set
        void SetStorage(Context& context, SymbolTypeStorageSet set);

        /// Get storage information
        /// \return
        SymbolTypeStorageSet GetStorage() const {
            return Storage;
        }

        // Target type
        SymbolType* type = nullptr;

        // Value information
        // SymbolTypeValueSet Value;

        // IL
        IL::IType* il = nullptr;

    private:
        // Storage information
        SymbolTypeStorageSet Storage;
    };
}