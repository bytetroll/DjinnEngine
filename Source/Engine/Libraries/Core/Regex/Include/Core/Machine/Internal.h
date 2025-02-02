// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <map>
#include <set>
#include <deque>
#include <stack>
#include <list>

namespace Djinn {
    namespace Regex {
        // Multimap support needs to be added to the engine.  For now,
        // use std::multimap. NCY, 5.17.2018
        // Also should add set support.
        template< typename A, typename B >
        using Multimap = std::multimap< A, B >;

        template< typename A >
        using Set = std::set< A >;

        template< typename A >
        using Deque = std::deque< A >;

        template< typename A >
        using Stack = std::stack< A >;

        template< typename A >
        using List = std::list< A >;
    }
}