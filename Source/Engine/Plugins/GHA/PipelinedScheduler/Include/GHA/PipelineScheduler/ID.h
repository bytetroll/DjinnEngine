//
// Created by Dev on 12/22/2017.
//
#pragma once

#include <Core/Unique.h>
#include <GHA/ID.h>

namespace Djinn {
    namespace GHA::PipelineScheduler {
        // Number IDs
        DJINN_UNIQUEID(TextureViewID, ResourceID);
        DJINN_UNIQUEID(PersistentID, ResourceID);
        DJINN_UNIQUEID(TextureUAVID, ResourceID);
    }

    /// Common hashes
    template<> struct Hasher<GHA::PipelineScheduler::TextureViewID> { USize operator()(const GHA::PipelineScheduler::TextureViewID& value) const { return Hasher<Int64>{}(value.Get()); }};
    template<> struct Hasher<GHA::PipelineScheduler::TextureUAVID> { USize operator()(const GHA::PipelineScheduler::TextureUAVID& value) const { return Hasher<Int64>{}(value.Get()); }};
}