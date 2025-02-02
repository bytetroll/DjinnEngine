//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/14/2017.
//
#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <set>
#include "StdAllocator.h"
#include "Shared.h"

namespace NyLang {
    template<typename T>
    using Allocator = std::allocator<T>;
    
    using String = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

    template<typename T>
    using Vector = std::vector<T, Allocator<T>>;

    template<typename T>
    using PtrVector = Vector<T*>;

    template<typename T>
    using List = std::list<T, Allocator<T>>;

    template<typename T>
    using PtrList = List<T*>;

    template<typename T>
    using Set = std::set<T, std::less<T>, Allocator<T>>;

    template<typename T, typename U>
    using Map = std::map<T, U, std::less<T>, Allocator<std::pair<const T, U>>>;

    template<typename T, typename U>
    using UnorderedMap = std::unordered_map<T, U, std::hash<T>, std::equal_to<T>, Allocator<std::pair<const T, U>>>;

    template<typename T, typename U>
    using UnorderedMultimap = std::unordered_multimap<T, U, std::hash<T>, std::equal_to<T>, Allocator<std::pair<const T, U>>>;
}

namespace std {
    /*template <>
    struct hash<NyLang::String>
    {
        std::size_t operator()(const NyLang::String& k) const
        {
            return hash<std:: string>{}(k.c_str());
        }
    };*/
}