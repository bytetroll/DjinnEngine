#pragma once

/// Force a definition to be inlined
/// TODO: Compiler specific
#define DJINN_FORCEINLINE  __attribute__((always_inline))

/// Suggest a definition to be inlined
#define DJINN_INLINE inline

/// Specify c declaration
#define DJINN_CDECL __attribute__ ((__cdecl__))

/// Source namespace helpers, header usage not recommended
#define DJINN_NS() using namespace Djinn
#define DJINN_NS2(NAME) using namespace Djinn; using namespace Djinn::NAME
#define DJINN_NS3(NAMEA, NAMEB) using namespace Djinn; using namespace Djinn::NAMEA; using namespace Djinn::NAMEA::NAMEB;

/// Stringify token
#define DJINN_STRINGIFY2(TOK) #TOK
#define DJINN_STRINGIFY(TOK) DJINN_STRINGIFY2(TOK)

/// Prevent copies of an object
#define DJINN_NOCOPY(NAME) NAME(const NAME&) = delete; NAME(const NAME&&) = delete; NAME& operator=(const NAME&) = delete; NAME& operator=(const NAME&&) = delete

/// Mark a class as static usage
#define DJINN_STATICCLASS(NAME) NAME() = delete; DJINN_NOCOPY(NAME)

/// Create a static block
#define DJINN_STATICBLOCK(NAME, BLOCK) struct __DjinnStaticBlock_##NAME { __DjinnStaticBlock_##NAME() { BLOCK; } } __instDjinnStaticBlock_##NAME;

/// Create a static block with deconstructor
#define DJINN_STATICBLOCK_DESTRUCT(NAME, BLOCK, DESTRUCT) struct __DjinnStaticBlock_##NAME { __DjinnStaticBlock_##NAME() { BLOCK; } ~__DjinnStaticBlock_##NAME() { DESTRUCT; } } __instDjinnStaticBlock_##NAME;

/// Constexpr class linking fix
#ifdef DJINN_COMPILER_GNU
#   define DJINN_STATICCONSTEXPR DJINN_EXPORT static constexpr
#else
#   define DJINN_STATICCONSTEXPR static constexpr
#endif

namespace Djinn {
    /// Sink a set of parameters
    template<typename... T>
    void Sink(T&&...) { }
}

// Migi namespace bullshit shortcuts.

// The formatting on these macro's is required to prevent most IDE"s from auto formatting
// containing code.  Migi... if you fucking touch these, I WILL GUT YOU LIKE A CORNISH HEN!
//                                                       migi: D:
#define DJINN_BEGIN_NS1( NS1 ) \
namespace NS1 {

#define DJINN_END_NS1( NS1 ) \
}

#define DJINN_BEGIN_NS2( NS1, NS2 ) \
namespace NS1 { \
namespace NS2 {

#define DJINN_END_NS2( NS1, NS2 ) \
} \
}

#define DJINN_BEGIN_NS3( NS1, NS2, NS3 ) \
namespace NS1 { \
namespace NS2 { \
namespace NS3 {

#define DJINN_END_NS3( NS1, NS2, NS3 ) \
} \
} \
}

#define DJINN_BEGIN_NS4( NS1, NS2, NS3, NS4 ) \
namespace NS1 { \
namespace NS2 { \
namespace NS3 { \
namespace NS4 {

#define DJINN_END_NS4(NS1, NS2, NS3, NS4) \
} \
} \
} \
}
