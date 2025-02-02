//
// Created by dev on 6/5/18.
//
#pragma once

namespace Djinn::Attributes {
    constexpr void* Ignore = nullptr;

/// VC++ __VA_ARGS__ fix
#define DJINN_EXPAND(x) x

#define DJINN_CONCAT_IMPL(x, y) x##y
#define DJINN_CONCAT(x, y) DJINN_CONCAT_IMPL( x, y )

#define Attr_DeclType_X(X) struct DJINN_CONCAT(__DHG_DCLTYPE_, __COUNTER__) { void Foo() { using namespace Djinn::Attributes; [[maybe_unused]] auto fuckoff = X; } };
#define Attr_DeclType_1(A) Attr_DeclType_X(A);
#define Attr_DeclType_2(A, B) Attr_DeclType_X(A);  Attr_DeclType_X(B);
#define Attr_DeclType_3(A, B, C) Attr_DeclType_X(A);  Attr_DeclType_X(B);  Attr_DeclType_X(C);
#define Attr_DeclType_4(A, B, C, D) Attr_DeclType_X(A);  Attr_DeclType_X(B);  Attr_DeclType_X(C);  Attr_DeclType_X(D);
#define Attr_DeclType_5(A, B, C, D, E) Attr_DeclType_X(A);  Attr_DeclType_X(B);  Attr_DeclType_X(C);  Attr_DeclType_X(D);  Attr_DeclType_X(E);
#define Attr_DeclType_6(A, B, C, D, E, F) Attr_DeclType_X(A);  Attr_DeclType_X(B);  Attr_DeclType_X(C);  Attr_DeclType_X(D);  Attr_DeclType_X(E);  Attr_DeclType_X(F);
#define Attr_DeclType_7(A, B, C, D, E, F, G) Attr_DeclType_X(A);  Attr_DeclType_X(C);  Attr_DeclType_X(C);  Attr_DeclType_X(D);  Attr_DeclType_X(E);  Attr_DeclType_X(F);  Attr_DeclType_X(G);
#define Attr_DeclType_N(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define Attr_DeclType__(...) DJINN_EXPAND(Attr_DeclType_N(__VA_ARGS__, Attr_DeclType_7, Attr_DeclType_6, Attr_DeclType_5, Attr_DeclType_4, Attr_DeclType_3, Attr_DeclType_2, Attr_DeclType_1)(__VA_ARGS__))

/// Workaround
//#undef Attr_DeclType__
//#define Attr_DeclType__(...)

/// Compiler annotations only part of DHG
#ifdef DJINN_DHG

    /// Annotation up to 7 overloads
#   define Attr_Paste(X) #X
#   define Attr_1(A) __attribute__((annotate(Attr_Paste(A))))
#   define Attr_2(A, B)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B))))
#   define Attr_3(A, B, C)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B)), annotate(Attr_Paste(C))))
#   define Attr_4(A, B, C, D)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B)), annotate(Attr_Paste(C)), annotate(Attr_Paste(D))))
#   define Attr_5(A, B, C, D, E)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B)), annotate(Attr_Paste(C)), annotate(Attr_Paste(D)), annotate(Attr_Paste(E))))
#   define Attr_6(A, B, C, D, E, F)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B)), annotate(Attr_Paste(C)), annotate(Attr_Paste(D)), annotate(Attr_Paste(E)), annotate(Attr_Paste(F))))
#   define Attr_7(A, B, C, D, E, F, G)  __attribute__((annotate(Attr_Paste(A)), annotate(Attr_Paste(B)), annotate(Attr_Paste(C)), annotate(Attr_Paste(D)), annotate(Attr_Paste(E)), annotate(Attr_Paste(F)), annotate(Attr_Paste(G))))
#   define Attr_N(_1,_2,_3,_4,_5,_6,_7,NAME,...) NAME
#   define Attr__(...) DJINN_EXPAND(Attr_N(__VA_ARGS__, Attr_7, Attr_6, Attr_5, Attr_4, Attr_3, Attr_2, Attr_1)(__VA_ARGS__))

/// Annotation helper
#   define Attr(...) Attr__(__VA_ARGS__)
#   define ClassAttr(...) class Attr__(__VA_ARGS__) DJINN_CONCAT(__DHG_ClassAttr, __COUNTER__) {};

#else

/// Annotation helper
#   define Attr(...) Attr_DeclType__(__VA_ARGS__)
#   define ClassAttr(...) Attr_DeclType__(__VA_ARGS__)

#endif
}
