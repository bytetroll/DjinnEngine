//
// Created by Dev on 3/29/2018.
//
#pragma once

#include "Info.h"

namespace Djinn::Async {
    struct JobGroup;

    struct JobPostOp {
        CounterOp op = CounterOp::eNone;
        UInt32 argument = 1;
    };

    struct ScheduledJob {
        bool isParameterized = false;
        ParameterizedScheduleInfo parameterizedInfo;
        ScheduleInfo info;
        JobGroup *group = nullptr;
        JobPostOp post;
        const char* name = nullptr;
    };

    struct YieldedJob {
        void *workerState = nullptr;
        void *worker = nullptr;
    };
}