//
// Created by Dev on 4/10/2018.
//
#pragma once

#include <GHA/Stage.h>

namespace Djinn::GHA::PipelineScheduler {
    struct DynamicRange {
        GHA::Stage stage = {};
        UInt32 offset = 0;
        UInt32 size = 0;
    };
}