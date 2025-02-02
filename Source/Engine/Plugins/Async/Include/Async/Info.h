//
// Created by Dev on 1/4/2018.
//
#pragma once

#include <Core/Atomic.h>
#include <Core/DynamicDelegate.h>
#include "CounterOp.h"
#include "ConditionalOp.h"

namespace Djinn::Async {
    struct ScheduleInfo {
        Core::Delegate<void()> delegate;
    };

    struct ParameterizedScheduleInfo {
        Core::DynamicDelegate delegate;
        Core::DynamicParameterStack stack;
    };
}
