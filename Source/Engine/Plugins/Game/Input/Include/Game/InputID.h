//
// Created by Dev on 1/15/2018.
//
#pragma once

#include <Core/Unique.h>

namespace Djinn::Game {
    // ID Aliases
    DJINN_UNIQUEID(MappingID, UInt64);
    DJINN_UNIQUEID(AxisID, UInt64);
}

namespace Djinn {
    /// Common hashes
    template<> struct Hasher<Game::MappingID> { USize operator()(const Game::MappingID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<Game::AxisID> { USize operator()(const Game::AxisID& value) const { return Hasher<Int64>{}(value.Get()); }};


}