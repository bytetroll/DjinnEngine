//
// Created by Dev on 11/14/2017.
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