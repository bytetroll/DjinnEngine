#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <cstring>
#include <limits.h>
#include <NyLang/Core/Hash.h>

namespace NyLang {
    // Token type
    LANG_ID(64, TokenID, tok);

    // Token Location
    // Defines the location of the parent token in the source code
    struct TokenLoc {
        TokenLoc() {}

        // Data
        unsigned Start = 0;
        unsigned End = 0;
        unsigned Line = 1;
        unsigned Char = 1;
    };

    // Token Value Type
    enum class TokenValueType {
        BOOL,
        FLOAT,
        DOUBLE,
        INT,
        UINT,
        INT64,
        UINT64,
        STRING
    };

    // Token Value
    // Some tokens have values associated with them
    union TokenValue {
        // Wrapper
        template <TokenValueType TT, class T> struct Wrapper {
            Wrapper() {}
            Wrapper(T Value) : Value(Value) {}

            // Implicit cast
            operator T&() {
                return Value;
            }

            // Base Type
            TokenValueType Type = TT;

            // Value
            T Value;
        };

        // SA Values
        TokenValue();
        TokenValue(bool Bool);
        TokenValue(float Float);
        TokenValue(double Double);
        TokenValue(signed Int);
        TokenValue(unsigned UInt);
        TokenValue(long long signed Int64);
        TokenValue(long long unsigned UInt64);
        TokenValue(const char* StrExt);

        // Copy Constructor
        TokenValue(const TokenValue& Other);

        // Some types require special deconstruction
        ~TokenValue();

        // Copy string
        void CopyString(const char* StrExt);

        // Type Wrapper
        TokenValueType TypeWrapper;

        // Data
        Wrapper<TokenValueType::BOOL, bool> Bool;
        Wrapper<TokenValueType::FLOAT, float> Float;
        Wrapper<TokenValueType::DOUBLE, double> Double;
        Wrapper<TokenValueType::INT, signed> Int;
        Wrapper<TokenValueType::UINT, unsigned> UInt;
        Wrapper<TokenValueType::INT64, long long signed> Int64;
        Wrapper<TokenValueType::UINT64, long long unsigned> UInt64;
        Wrapper<TokenValueType::STRING, char*> Str;
    };

    // Token
    // Defines a token with an associated value and location
    struct Token {
        Token();
        Token(TokenID ID, TokenLoc Span, TokenValue Value = {});
        Token(const Token& Other);

        // Debug token
        explicit Token(TokenID ID);

        // Compare to other token
        bool operator==(const Token& Other);

        // Compare to tokenID
        bool operator==(TokenID Tok);

        // Compare to tokenID
        bool operator!=(TokenID Tok);

        // Data
        TokenID ID;
        TokenLoc Loc;
        TokenValue Value;
    };
}