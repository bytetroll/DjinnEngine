#pragma once

#include <exception>
#include <iostream>

namespace NyLang {
    static void Assert(bool expectedTrue, const char* msg) {
        if (!expectedTrue) {
            std::cout << "Assertion failed: " << msg;
            throw std::exception();
        }
    }
}