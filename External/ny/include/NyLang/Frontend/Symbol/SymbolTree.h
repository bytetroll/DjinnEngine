#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//


#include <NyLang/Core/BitSet.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/Shared.h>
#include <NyLang/Native/Symbols/Meta.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include <list>
#include <NyLang/Core/PrettyStream.h>
#include <unordered_map>
#include <set>
#include "SymbolPair.h"

namespace NyLang {
    class SymbolTree;

    // Dummy predicate
    template<typename T>
    static bool __SymbolTreePredicate(T*) { return true; }

    // Dummy class predicate
    static ClassID __SymbolTreeClassPredicate(SymbolComponent*ptr) { return ptr->GetClassID(); }

    // Search flags
    enum class SymbolTreeSearchFlag {
        eNoParent = 1, // Prevent parent tree searches
        eExternal = (int) eNoParent << 1, // Does not own this tree
        eLayered = (int) eExternal << 1, // Internal flag
    }; LANG_ENUM_BITSET(SymbolTreeSearchFlag);

    // Symbol search candidate
    template<typename T = SymbolComponent>
    struct SymbolSearch {
        SymbolSearch() {

        }

        template<typename TT>
        explicit SymbolSearch(const SymbolSearch<TT> &other) {
            this->visibility = other.visibility;
            this->remoteVisibility = other.remoteVisibility;
            this->pair = SymbolPair<T>(other.pair.symbol, OuterCast<T>(other.pair.component));
            this->owner = other.owner;
        }

        bool operator==(const SymbolSearch<T> &other) {
            return pair == other.pair;
        }

        SymbolVisibility visibility;
        SymbolVisibility remoteVisibility;
        SymbolPair<T> pair;
        SymbolTree *owner;
    };

    class SymbolTree : public IUnknown {
    public:
        LANG_CLASSID("SymbolTree");

        SymbolTree(SymbolTree *Parent, Symbol *Owner = nullptr, const SymbolTreeSearchFlagSet &searchFlags = {});
        ~SymbolTree();

        /// Overrides
        virtual ClassID GetClassID() override;

        SymbolTree* Push(Context& context);

        // Create branch
        SymbolTree* CreateBranch(Context& context);

        // Merge branch
        void MergeBranch(SymbolTree*Branch);

        // Add symbol
        void AddSymbol(SymbolVisibility Visibility, Symbol*New);

        // Add adjacent tree
        void AddAdjacentTree(SymbolVisibility Visibility, SymbolTree*New);

        // Find symbol by type(s) and name, all types MUST inherit from SymbolNamed
        template<class T, typename F =  bool (*)(T*)>
        std::list<SymbolSearch<T>> Find(const String &Name, const SymbolTreeSearchFlagSet &flags = {}, F predecate = __SymbolTreePredicate<T>) {
            std::list<SymbolSearch<T>> candidates;

            // Combine flags
            SymbolTreeSearchFlagSet combinedFlags = searchFlags + flags;

            // Get all symbols
            auto &&symbols = NamedSymbols.equal_range(Name);
            for (auto it = symbols.first; it != symbols.second; ++it) {
                auto &&com = it->second.second->GetComponent<T>();
                if (com && predecate(com)) {
                    SymbolSearch<T> search;
                    search.visibility = it->second.first;
                    search.remoteVisibility = it->second.first;
                    search.pair = {it->second.second, com};
                    search.owner = this;
                    candidates.push_back(search);
                }
            }

            // Search parent
            if (!(combinedFlags & SymbolTreeSearchFlag::eNoParent) && Parent) {
                auto Ptr = Parent->Find<T>(Name, combinedFlags, predecate);
                candidates.insert(candidates.end(), Ptr.begin(), Ptr.end());
            }

            // Try adjacent trees
            for (auto &Tree : Adjacents) {
                auto Ptr = Tree.second->Find<T>(Name, combinedFlags + SymbolTreeSearchFlag::eLayered, predecate);
                for (auto &&it : Ptr) {
                    // Downgrade remote visibility if external or layered
                    if ((flags & SymbolTreeSearchFlag::eExternal || flags & SymbolTreeSearchFlag::eLayered) && Tree.first < it.remoteVisibility) {
                        it.remoteVisibility = Tree.first;
                    }

                    // Privates are inaccessible
                    switch (it.remoteVisibility) {
                        case SymbolVisibility::ePrivate:
                            it.remoteVisibility = SymbolVisibility::eInaccessible;
                            break;
                    }
                }
                candidates.insert(candidates.end(), Ptr.begin(), Ptr.end());
            }

            //

            // Post processing
            for (auto &&it : candidates) {
                // All non publics are inaccessible from external
                if (flags & SymbolTreeSearchFlag::eExternal) {
                    switch (it.remoteVisibility) {
                        case SymbolVisibility::eProtected:
                        case SymbolVisibility::ePrivate:
                            it.remoteVisibility = SymbolVisibility::eInaccessible;
                            break;
                    }
                }
            }

            // ...
            candidates.unique();
            return candidates;
        }

        /// Search with importance resolve
        /// \param Name
        /// \param tree
        /// \param flags
        /// \return
        template<typename T, typename F = ClassID(*)(SymbolComponent*)>
        std::list<SymbolSearch<T>> FindImportanceResolve(const String &Name, const SymbolTreeSearchFlagSet &flags = {}, F &&predicate = __SymbolTreeClassPredicate) {
            auto &&symbols = Find<T>(Name, flags);

            // Get max layer
            Map<ClassID::Type, int> sets;
            for (auto it = symbols.begin(); it != symbols.end(); it++) {
                ClassID id = predicate(it->pair.component);
                sets[id.Get()] = std::max(it->pair.symbol->GetAnchor().tree->layer, sets[id.Get()]);
            }

            // Eliminate lower layers
            symbols.remove_if([&](auto &&it) {
                return it.pair.symbol->GetAnchor().tree->layer < sets[predicate(it.pair.component)];
            });

            return symbols;
        }

        // Pretty print
        // Returns the amount of layers
        int PrettyPrint(PrettyStream &stream);

        // Is this tree a child of?
        bool IsChildOf(SymbolTree *tree) {
            if (Parent) {
                if (Parent == tree) {
                    return true;
                }
                return Parent->IsChildOf(tree);
            }
            return false;
        }

        // Current layer of this tree
        // Used for improtance based symbol selection
        int layer = 0;

        // Search flags
        SymbolTreeSearchFlagSet searchFlags;

        // All symbols at this level
        std::list<std::pair<SymbolVisibility, Symbol*>> Symbols;

        // Named symbol lookup
        UnorderedMultimap<String, std::pair<SymbolVisibility, Symbol*>> NamedSymbols;

        // Parent tree
        // May be void
        SymbolTree *Parent = nullptr;

        // Adjacent Trees
        // Some trees may be visible from this tree, such as inherited objects or imported namespaces
        std::list<std::pair<SymbolVisibility, SymbolTree*>> Adjacents;

        // Child trees
        std::list<SymbolTree*> Children;

        // Owner of this tree
        // Can (easily) be void
        Symbol *Owner = nullptr;

        // Branch owner
        // Can be void
        SymbolTree *BranchOwner = nullptr;

        // Insertion tree
        SymbolTree* insertionTree = nullptr;

    private:
        // Checks if types inherit from base
        template<class BASE, class T0>
        static constexpr bool IsSymbolBase() {
            return std::is_base_of<BASE, T0>::value;
        }

        // Checks if types inherit from base
        template<class BASE, class T0, class T1, class... TX>
        static constexpr bool IsSymbolBase() {
            return IsSymbolBase<BASE, T0>() && IsSymbolBase<BASE, T1, TX...>();
        }
    };
}