#pragma once

#include "Common.h"
#include <string>

namespace Djinn {
    /// Generic exception
    struct EAssertionFailed {
        // Empty
    };

    /// External error output
    /// \param str
    extern void CErr(const char *str);

    /// Print expansion
    static void __AssertPrint() {}

    /// Print expansion
    template<typename T>
    static void __AssertPrint(T&& a) {
        CErr(std::to_string(a).c_str());
    }

    /// Print expansion
    static void __AssertPrint(const char* str) {
        CErr(str);
    }

    /// Print expansion
    static void __AssertPrint(char* str) {
        CErr(str);
    }

    /// Print expansion
    template<typename T0, typename T1, typename... TX>
    static void __AssertPrint(T0 &&a0, T1 &&a1, TX &&... ax) {
        __AssertPrint(a0);
        __AssertPrint(a1, ax...);
    }

    /// Debug assertion, no lazy argument evaluation
    /// \param condition
    /// \param messages
    template<typename... TX>
    static void __Assert(TX &&... messages) {
#ifdef DJINN_DEBUG
        __AssertPrint("Assertion failed, ");
        __AssertPrint(messages...);
        __AssertPrint("\n");
        DJINN_THROW EAssertionFailed();
#endif
    }

    /// Assertion, no lazy argument evaluation
    /// \param condition
    /// \param messages
    template<typename... TX>
    static void Always(bool condition, TX &&... messages) {
        if (!condition) {
            __AssertPrint("Assertion failed, ");
            __AssertPrint(messages...);
            __AssertPrint("\n");
            DJINN_THROW EAssertionFailed();
        }
    }

    /// Assert helper
#ifdef DJINN_DEBUG
#define DJINN_ASSERT(COND, ...) (void)((COND) || (Djinn::__Assert(__FILE__, " : ", DJINN_STRINGIFY(__LINE__), "\n\t", #COND, "\n\t\"", ##__VA_ARGS__, "\""),0))
#else
#define DJINN_ASSERT(COND, ...)
#endif
}