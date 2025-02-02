#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Symbol/Symbol.h>
#include <NyLang/Backend/IL/ValueAddressPair.h>

namespace NyLang {
    struct SymbolTypeInfo;
    class SymbolValueInfo;
    class SymbolTree;
    namespace IL {
        class IValue;
    }

    /// Collection mode
    enum class ValueCollectionMode {
        // Collect the value
        COLLECT,

        // Dont collect the value
        NOCOLLECT,

        // Runtime collection
        RUNTIME
    };

    /// Value
    struct SymbolValue : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolValue");

        /// Create component
        explicit SymbolValue(SymbolTypeInfo* TypeInfo, const IL::ValueAddressPair& il);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        virtual bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;
        virtual IL::ValueAddressPair &GetIL(Context &context);

        // Type of value
        SymbolTypeInfo* TypeInfo = nullptr;

        // Static tree
        SymbolTree* staticTree = nullptr;

        // Is this value constant?
        bool isConstant = false;

    protected:
        /// Create component
        explicit SymbolValue(const ClassID& cid, SymbolTypeInfo* TypeInfo, bool isConstant);

        // IL
        IL::ValueAddressPair il;
    };

    // Value info
    struct SymbolValueInfo : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolValueInfo");

        /// Create component
        explicit SymbolValueInfo(Context& context, SymbolTypeInfo* Value, const IL::ValueAddressPair& il);

        /// Create component
        explicit SymbolValueInfo(SymbolValue* Value);

        /// Create component
        explicit SymbolValueInfo(Context& context, SymbolValueInfo* Info);

        /// Overrides
        void PrettyPrint(PrettyStream& stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Value
        SymbolValue* value = nullptr;

        // Is this value assignable (Not temporary)?
        bool isAssignable = false;

        // Should this value be collected?
        ValueCollectionMode collectMode = ValueCollectionMode::NOCOLLECT;
    };
}