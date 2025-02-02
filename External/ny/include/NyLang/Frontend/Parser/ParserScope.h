//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Hash.h>
#include <unordered_set>

namespace NyLang {
    LANG_ID(16, ParserScope, pscope);

    /// Parser scope stack
    class ParserScopeStack {
        static constexpr std::uint64_t kMaxValues = 16;

    public:
        /// Construct
        ParserScopeStack() {

        }

        /// Construct
        /// \param scope
        ParserScopeStack(const ParserScope& scope) {
            *this += scope;
        }

        /// Construct
        /// \param other
        ParserScopeStack(const ParserScopeStack& other) = default;

        /// Append scope
        /// \param scope
        /// \return
        ParserScopeStack operator+(const ParserScope& scope) const {
            return ParserScopeStack(*this) += scope;
        }

        /// Append scope
        /// \param scope
        /// \return
        ParserScopeStack& operator+=(const ParserScope& scope) {
            std::uint16_t* p = scopes;
            for (std::uint64_t i = 0; i < kMaxValues; i++, p++) {
                if (*p == 0) {
                    *p = scope;
                    if (i + 1 > count) {
                        count = i + 1;
                    }
                    break;
                }
            }
            return *this;
        }

        /// Remove scope
        /// \param scope
        /// \return
        ParserScopeStack operator-(const ParserScope& scope) const {
            return ParserScopeStack(*this) -= scope;
        }

        /// Remove scope
        /// \param scope
        /// \return
        ParserScopeStack& operator-=(const ParserScope& scope) {
            std::uint16_t* p = scopes;
            for (std::uint64_t i = 0; i < kMaxValues && *p; i++, p++) {
                if (*p == scope) {
                    *p = scopes[count-1];
                    scopes[count-1] = 0;
                    count--;
                    break;
                }
            }
            return *this;
        }

        /// Has scope?
        /// \param scope
        /// \return
        bool operator&(const ParserScope& scope) const {
            const std::uint16_t* p = scopes;
            for (std::uint64_t i = 0; i < count; i++, p++) {
                if (*p == scope) {
                    return true;
                }
            }
            return false;
        }

    private:
        std::uint16_t scopes[kMaxValues] = {};
        std::uint16_t count = 0;
    };
}
