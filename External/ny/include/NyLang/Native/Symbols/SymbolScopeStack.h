#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <vector>
#include <climits>
#include <memory>
#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/StdTypes.h>

namespace NyLang {
    class Symbol;

    template<class T>
    class SymbolScopeStack {
    public:
        SymbolScopeStack() {
            // ...
        }
        SymbolScopeStack(const SymbolScopeStack& stack) : scopes(stack.scopes), offset(stack.scopes.size()) {
            // ...
        }

        // Create branch
        /*SymbolScopeStack* CreateBranch() {
            return Make<SymbolScopeStack<T>>(*this);
        }*/

        // Merge branch
        void MergeBranch(SymbolScopeStack* Branch) {
            scopes.insert(scopes.end(), Branch->scopes.begin() + offset, Branch->scopes.end());
        }

        // Push a scope onto the stack
        void PushScope(T scope, Symbol* value = nullptr) {
            scopes.push_back(std::make_pair(scope, value));
        }

        // Pop the scope stack
        void PopScope() {
            scopes.pop_back();
        }

        // Scope branch
        template <class R, typename F>
        R Scoped(T scope, Symbol* value, F functor) {
            PushScope(scope, value);
            R ret = functor();
            PopScope();

            // ...
            return ret;
        }

        // Check if within scope, returns depth (0 -> Not found, 1 .. n -> Layer )
        unsigned int IsWithinScope(T scope) {
            for (int i = scopes.size() - 1; i >= 0; i--) {
                if (scopes[i].first == scope)
                    return (unsigned int) (scopes.size() - i);
            }
            return 0;
        }

        // Templated scope check
        template<typename T0, typename T1, typename... TX>
        unsigned int IsWithinScope(T0 A, T1 B, TX... X) {
            if (unsigned int D = IsWithinScope(A))
                return D;
            return IsWithinScope<T1, TX...>(B, X...);
        }

        // Get outer scope symbol
        template <class S>
        S* GetOuterSymbol(T scope, int layerLimit = INT_MAX - 1) {
            for (int i = (int)scopes.size() - 1; ((i >= 0) && ((int)scopes.size() - 1 <= layerLimit)); i--) {
                if (scopes[i].first == scope) {
                    return OuterCast<S>(scopes[i].second);
                }
            }
            return nullptr;
        }

    private:
        // Branch offset
        int offset = 0;

        // Stack
        Vector<std::pair<T, Symbol*>> scopes;
    };
}