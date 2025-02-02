//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <chrono>

namespace NyLang {
    // Benchmarks action functor and returns the duration in seconds
    template <typename F>
    double Benchmark(F&& action) {
        auto begin = std::chrono::system_clock::now();

        // User action
        action();

        // Calculate time
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - begin).count() / 1E9;
    }
}
