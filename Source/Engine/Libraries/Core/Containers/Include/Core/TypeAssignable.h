#pragma once

namespace Djinn {
    namespace Core {
/*#define DJINN_TYPEASSIGNABLE(CLASS) \
    CLASS() = default; \
    template<typename... A> \
    explicit CLASS(A &&... args) { \
        Assign(args...); \
    } \
    void Assign() { \
    } \
    template<typename A0, typename A1, typename... AX> \
    void Assign(A0 &&a0, A1 &&a1, AX &&... ax) { \
        Assign(a0); \
        Assign(a1, ax...); \
    } \
    void Assign(const CLASS& other) { *this = other; }

#define DJINN_TYPEASSIGN(TYPE, NAME) \
    void Assign(TYPE __arg) { NAME = __arg; }*/
    }
}