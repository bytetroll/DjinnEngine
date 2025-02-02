//
// Created by Dev on 7/14/2018.
//
#pragma once

#include <Core/Unique.h>

namespace Djinn::Data {
    /// ID Aliases
    DJINN_UNIQUEID(WorldFeatureID, UInt64);

    /// World feature helper
#define DJINN_WORLD_FEATURE(NAME) static constexpr auto k##NAME = Djinn::Data::WorldFeatureID(#NAME##_crc64);
}

namespace Djinn {
    /// Common hashes
    template<> struct Hasher<Data::WorldFeatureID> { USize operator()(const Data::WorldFeatureID& value) const { return Hasher<UInt64>{}(value.Get()); }};
}