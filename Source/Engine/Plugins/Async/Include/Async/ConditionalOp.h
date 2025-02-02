//
// Created by Dev on 1/4/2018.
//
#pragma once

namespace Djinn::Async {
    enum class ConditionalOp {
        eNone = 0,
        eLess = 1,
        eLessEq = 2,
        eMore = 3,
        eMoreEq = 4,
        eEqual = 5,
        eNotEqual = 6
    };
}