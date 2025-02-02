/* Used in generated sources */

#include <cstdlib>
#include <Platform/LibraryMemoryAPI.h>

DJINN_NS2(Platform);

/// Hooks
DJINN_EXPORT void* DjinnMallocHookUser = nullptr;
DJINN_EXPORT LibraryMemoryAPI::MallocHook::Type DjinnMallocHook = []( void* user, USize size ) {
    return malloc(size);
};

DJINN_EXPORT void* DjinnFreeHookUser = nullptr;
DJINN_EXPORT LibraryMemoryAPI::FreeHook::Type DjinnFreeHook = [](void* user, void* block) {
    free(block);
};

/// Proxies

/*void *operator new(size_t size) {
    return DjinnMallocHook(DjinnMallocHookUser, size);
}

void *operator new[](size_t size) {
    return DjinnMallocHook(DjinnMallocHookUser, size);
}

void operator delete(void *ptr) {
    DjinnFreeHook(DjinnFreeHookUser, ptr);
}

void operator delete(void *ptr, std::size_t size) {
    DjinnFreeHook(DjinnFreeHookUser, ptr);
}

void operator delete[](void *ptr) {
    DjinnFreeHook(DjinnFreeHookUser, ptr);
}

void operator delete[](void *ptr, std::size_t size) {
    DjinnFreeHook(DjinnFreeHookUser, ptr);
}*/