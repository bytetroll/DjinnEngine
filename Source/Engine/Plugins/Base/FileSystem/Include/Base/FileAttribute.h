//
// Created by Dev on 10/22/2017.
//
#pragma once

#include <Core/BitSet.h>

namespace Djinn::Base {
    enum class FileAttribute {
        eDirectory = 1,
    }; DJINN_CORE_BITSET(FileAttribute);
}