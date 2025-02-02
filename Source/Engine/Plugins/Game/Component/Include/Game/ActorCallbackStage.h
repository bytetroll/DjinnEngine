//
// Created by Dev on 8/18/2018.
//
#pragma once

#include <Core/Unique.h>

namespace Djinn::Game {
    /// ID Aliases
    DJINN_UNIQUEID(ActorCallbackStage, UInt32);

    /// Default stages
    static constexpr ActorCallbackStage kDefaultActorCallback = "default"_id;
}

namespace Djinn {
    /// Common hashes
    template<> struct Hasher<Game::ActorCallbackStage> { USize operator()(const Game::ActorCallbackStage& value) const { return Hasher<Int64>{}(value.Get()); }};
}