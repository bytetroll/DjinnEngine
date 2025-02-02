#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/Shared.h>

namespace NyLang {
    class SymbolComponent;
    class Symbol;

    /// Symbol pair
    template <typename T = SymbolComponent>
    struct SymbolPair {
        SymbolPair() {

        }

        /// Create pair
        SymbolPair(Symbol* symbol, T* com) : symbol(symbol), component(com) {

        }

        /// Is valid?
        /// \return
        operator bool() {
            return (symbol != nullptr && component != nullptr);
        }

        /// Comparison
        /// \param other
        /// \return
        bool operator==(const SymbolPair<T>& other) const {
            return symbol == other.symbol && component == other.component;
        }

        /// Access component
        /// \return
        T* operator->() const {
            return component;
        }

        Symbol* symbol = nullptr;
        T* component = nullptr;
    };
}