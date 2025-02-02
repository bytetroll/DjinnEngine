//
// Created by Dev on 11/14/2017.
//
#pragma once

#include <cstring>

namespace NyLang {
    namespace SimpleAllocator {
        extern void *Allocate(std::size_t size);
        extern void Free(void *ptr);
    }
}