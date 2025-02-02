#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

namespace NyLang {
    template <class T>
    class BitSet {
    public:
        BitSet(T Value = T()) : Value(Value) {

        }

        // Implicit
        operator const T&() const {
            return Value;
        }

        // Add value
        BitSet& operator+=(const T& V) {
            Value = (T)((int)V | (int)Value);
            return *this;
        }

        // Add value
        BitSet operator+(const T& V) const {
            return BitSet((T)((int)V | (int)Value));
        }

        // Add set
        BitSet& operator+=(const BitSet<T>& V) {
            Value = (T)((int)V.Value | (int)Value);
            return *this;
        }

        // Add set
        BitSet operator+(const BitSet<T>& V) const {
            return BitSet((T)((int)V.Value | (int)Value));
        }

        // Subtract set
        BitSet operator-(const T& V) const {
            return BitSet((T)((int)Value & ~(int)V));
        }

        // Subtract set
        BitSet operator-(const BitSet<T>& V) const {
            return BitSet((T)((int)Value & ~(int)V.Value));
        }

        // Subtract set
        BitSet& operator-=(const T& V) {
            Value = (T)((int)Value & ~(int)V);
            return *this;
        }

        // Subtract set
        BitSet& operator-=(const BitSet<T>& V) {
            Value = (T)((int)Value & ~(int)V.Value);
            return *this;
        }

        // Test value
        bool operator&(const T& V) const {
            return (int)Value & (int)V;
        }

        // Test value
        bool operator&(const BitSet& set) const {
            return (int)Value & (int)set.Value;
        }

        // Is empty?
        bool IsEmpty() {
            return (int)Value == 0;
        }

        // Comparison
        bool operator==(const BitSet& set) const {
            return Value == set.Value;
        }

        // Comparison
        bool operator!=(const BitSet& set) const {
            return Value != set.Value;
        }

    private:
        T Value;
    };

    // Declare bit set operations for enum
#define LANG_ENUM_BITSET(NAME) \
        using NAME##Set = BitSet<NAME>; \
        static NAME##Set operator|(const NAME& L, const NAME& R) { return NAME##Set(L) + R; }
}