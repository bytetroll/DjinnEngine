#pragma once

namespace Djinn::Platform::LibraryMemoryAPI {
    struct MallocHook {
        using Type = void *(*)(void* user, USize size);
        static constexpr const char *kName = "DjinnMallocHook";
        static constexpr const char *kUserName = "DjinnMallocHookUser";
    };

    struct FreeHook {
        using Type = void (*)(void* user, void *block);
        static constexpr const char *kName = "DjinnFreeHook";
        static constexpr const char *kUserName = "DjinnFreeHookUser";
    };
}