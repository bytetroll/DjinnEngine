//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Hash.h>
#include <unordered_set>
#include <NyLang/Core/Shared.h>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include <NyLang/Frontend/Symbol/SymbolPair.h>

namespace NyLang {
    LANG_ID(16, SemanticScope, sscope);
    struct SemanticScopeEntry {
        SemanticScopeEntry() {

        }

        template<typename T = SymbolComponent>
        SemanticScopeEntry(SemanticScope scope, const SymbolPair<T> other) : scope(scope) {
            pair.symbol = other.symbol;
            pair.component = other.component;
        }

        SemanticScopeEntry(SemanticScope scope) : scope(scope) {
            pair = {nullptr, nullptr};
        }

        SemanticScopeEntry(SemanticScope scope, Symbol* other) : scope(scope) {
            pair = {other, nullptr};
        }

        SemanticScopeEntry(SemanticScope scope, SymbolComponent* other) : scope(scope) {
            pair = {nullptr, other};
        }

        SemanticScope scope;
        SymbolPair<SymbolComponent> pair;
    };
    
    /// Semantic scope stack
    class SemanticScopeStack {
        static constexpr std::uint64_t kMaxValues = 16;

    public:
        
        /// Construct
        SemanticScopeStack() {
        }

        /// Construct
        /// \param scope
        SemanticScopeStack(const SemanticScopeEntry& entry) {
            *this += entry;
        }

        /// Construct
        /// \param other
        SemanticScopeStack(const SemanticScopeStack& other) = default;

        /// Append scope
        /// \param scope
        /// \return
        SemanticScopeStack operator+(const SemanticScopeEntry& entry) const {
            return SemanticScopeStack(*this) += entry;
        }

        /// Append scope
        /// \param scope
        /// \return
        SemanticScopeStack operator+(const SemanticScope& scope) const {
            return SemanticScopeStack(*this) += scope;
        }

        /// Append scope
        /// \param scope
        /// \return
        SemanticScopeStack& operator+=(const SemanticScopeEntry& entry) {
            SemanticScopeEntry* p = scopes;
            for (std::uint64_t i = 0; i < kMaxValues; i++, p++) {
                if (p->scope == entry.scope) {
                    *p = entry;
                    break;
                }
                if (!p->scope) {
                    *p = entry;
                    if (i + 1 > count) {
                        count = i + 1;
                    }
                    break;
                }
            }
            return *this;
        }

        /// Append scope
        /// \param scope
        /// \return
        SemanticScopeStack& operator+=(const SemanticScope& scope) {
            return *this += SemanticScopeEntry(scope);
        }

        /// Remove scope
        /// \param scope
        /// \return
        SemanticScopeStack operator-(const SemanticScope& scope) const {
            return SemanticScopeStack(*this) -= scope;
        }

        /// Remove scope
        /// \param scope
        /// \return
        SemanticScopeStack& operator-=(const SemanticScope& scope) {
            SemanticScopeEntry* p = scopes;
            for (std::uint64_t i = 0; i < kMaxValues && p->scope; i++, p++) {
                if (p->scope == scope) {
                    *p = scopes[count-1];
                    scopes[count-1].scope = {};
                    count--;
                    break;
                }
            }
            return *this;
        }

        /// Has scope?
        /// \param scope
        /// \return
        bool operator&(const SemanticScope& scope) const {
            const SemanticScopeEntry* p = scopes;
            for (std::uint64_t i = 0; i < count; i++, p++) {
                if (p->scope == scope) {
                    return true;
                }
            }
            return false;
        }

        /// Get scope?
        /// \param scope
        /// \return
        template<typename T = SymbolComponent>
        SymbolPair<T> Get(const SemanticScope& scope) const {
            const SemanticScopeEntry* p = scopes;
            for (std::uint64_t i = 0; i < count; i++, p++) {
                if (p->scope == scope) {
                    T* ptr = std::is_same_v<T, SymbolComponent> ? static_cast<T*>(p->pair.component) : OuterCast<T>(p->pair.component);
                    if (ptr || (std::is_same_v<T, SymbolComponent> && p->pair.symbol)) {
                        return {p->pair.symbol, ptr};
                    }
                }
            }
            return {};
        }

    private:
        SemanticScopeEntry scopes[kMaxValues];
        std::uint16_t count = 0;
    };
}
