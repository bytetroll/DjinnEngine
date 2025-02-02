//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/ID.h>

namespace NyLang {
    /// Dependency ordering
    enum class DependencyOrder {
        eBefore,
        eAfter
    };

    /// Dependency wrapper
    struct Dependency {
        /// Construct
        Dependency() {

        }

        /// Construct
        /// \param type
        /// \param cid
        Dependency(DependencyOrder type, ActionID a, ActionID b) : type(type), a(a), b(b) {

        }

        // Type of this dependency
        DependencyOrder type = {};

        // ActionIDs of this dependency
        ActionID a;
        ActionID b;
    };
}