//
// Created by Dev on 3/20/2018.
//
#pragma once

#include <Core/BitSet.h>

namespace Djinn::Base {
    enum class FileSystemFlag {
        eIndexing = 1,
    }; DJINN_CORE_BITSET(FileSystemFlag);
}