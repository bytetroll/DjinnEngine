#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <list>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include "Meta.h"

namespace NyLang {
    class SymbolInstantiation;
    namespace IL {
        class Class;
    }

    // Object
    struct SymbolObject : public SymbolComponent {
    public:
        LANG_CLASSID("SymbolObject");

        /// Create component
        explicit SymbolObject(Node* body, const SymbolObjectStorageSet& storage);

        /// Create component
        explicit SymbolObject(SymbolObject* other);

        /// Overrides
        void PrettyPrint(PrettyStream &stream) override;
        SymbolComponent *Copy(Context &context) override;
        bool Compare(SymbolComponent*other) override;
        virtual ClassID GetClassID() override;

        // Name of this object
        String name;

        // Bases
        std::list<SymbolObject*> Bases;

        // Traits
        std::list<SymbolObject*> Traits;

        // Object type
        SymbolObjectType type;

        // Storage
        SymbolObjectStorageSet storage;

        // Body node
        Node* body = nullptr;

        // IL
        IL::Class* il = nullptr;

        // Instantiation
        SymbolInstantiation* instantiation = nullptr;
    };
}