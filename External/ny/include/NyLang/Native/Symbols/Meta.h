#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/BitSet.h>
#include <NyLang/Frontend/Token/Token.h>

namespace NyLang {
    // Symbol visibility
    // Used in namespace imports
    enum class SymbolVisibility {
        // Some symbols are hidden
        eHidden = 1,

        eInaccessible = (int)eHidden << 1,
        ePrivate = (int)eInaccessible << 1,
        eProtected = (int)ePrivate << 1,
        ePublic = (int)eProtected << 1,
    };

    // Object Storage
    enum class SymbolObjectStorage : unsigned {
        eWrapper = 1,
        eStatic = (int)eWrapper << 1,
    }; LANG_ENUM_BITSET(SymbolObjectStorage);

    // Object type
    enum class SymbolObjectType : unsigned {
        eClass,
        eStruct,
        eTrait,
        eEnum
    };

    // Type Storage
    enum class SymbolTypeStorage : unsigned {
        eConst = 1,
        eImmutable = (int)eConst << 1,
        eRef = (int)eImmutable << 1,
        eForward = (int)eRef << 1,
        eOut = (int)eForward << 1,
        eReadOnly = (int)eOut << 1,
        eWriteOnly = (int)eReadOnly << 1,
        eAccessOnly = (int)eWriteOnly << 1,
        eFuture = (int)eAccessOnly << 1,
        eType = (int)eFuture << 1,
        eAuto = (int)eType << 1,
        eStatic = (int)eAuto << 1,
    }; LANG_ENUM_BITSET(SymbolTypeStorage);

    // FunctionType Storage
    enum class SymbolFunctionStorage : unsigned {
        eVirtual = 1,
        eDVirtual =  (int)eVirtual << 1,
        ePropertyGet = (int)eDVirtual << 1,
        ePropertySet = (int)ePropertyGet << 1,
        eOverride = (int)ePropertySet << 1,
        eConst = (int)eOverride << 1,
        eImmutable = (int)eConst << 1,
        ePure = (int)eImmutable << 1,
        eStatic = (int)ePure << 1,
    }; LANG_ENUM_BITSET(SymbolFunctionStorage);

    // FunctionType decoration
    enum class SymbolFunctionDecoration : unsigned {
        eNone = 0,
        eConstructor = 1,
        eDeconstructor = (int)eConstructor << 1,
        eOperator = (int)eDeconstructor << 1,
        eLOperator = (int)eOperator << 1,
        eROperator = (int)eLOperator << 1,
    };

    // Type Value
    /*enum class SymbolTypeValue : unsigned {
        // Runtime value
        eRuntime = 1,

        // Type value
        eType = (int)eRuntime << 1,

        // Static value (Compile time)
        eStatic = (int)eType << 1,

        // All types
        eAll = 0xffffffff
    }; LANG_ENUM_BITSET(SymbolTypeValue);*/
}