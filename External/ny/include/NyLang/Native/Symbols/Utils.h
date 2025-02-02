#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/BitSet.h>
#include <list>
#include <NyLang/Frontend/Symbol/SymbolPair.h>
#include <NyLang/Frontend/Token/Token.h>
#include <NyLang/Core/Context.h>

namespace NyLang {
    class IMessage;
    class SymbolType;
    struct SymbolTypeInfo;
    class SymbolValueInfo;

    // Symbol Helper
    // Contains common operations
    class SymbolHelper {
        SymbolHelper() = delete;

    public:
        // Resolve type from symbol
        static SymbolPair<SymbolTypeInfo> ResolveTypeInfo(Symbol* Symbol);

        // Resolve type from symbols
        static std::list<SymbolPair<SymbolTypeInfo>> ResolveTypeInfos(const std::list<Symbol*>& Symbol);

        // Resolve value from symbol
        static SymbolPair<SymbolValueInfo> ResolveValueInfo(Symbol*Symbol);

        // Resolve value from symbols
        static std::list<SymbolPair<SymbolValueInfo>> ResolveValueInfos(const std::list<Symbol*> &Symbol);

        // Resolve type or value from symbol
        static SymbolPair<SymbolTypeInfo> ResolveTypeOrValueTypeInfo(Symbol*Symbol);

        // Check if a symbol can be casted to another symbol
        static IMessage* CanCast(Context& context, const Token& loc, SymbolTypeInfo* from, SymbolTypeInfo* to, bool isInitialization = false);

        // Check if a symbol can be casted to another symbol
        static IMessage* CanCast(Context& context, const Token& loc, Symbol* from, Symbol* to, bool isInitialization = false);

        // Is scalar type
        static bool IsScalarType(SymbolType* type);
    };
}