//
// Created by Dev on 5/13/2018.
//

#include <Base/MemoryCallbacks.h>
#include <Core/Common.h>
#include <Base/Detail/PluginMemory.h>
#include <malloc.h>

DJINN_NS2(Base);

void *Djinn::Base::MallocCallback(void* user, USize size) {
    // Allocate
    void *block = malloc(size);

    // Profile
    auto memory = static_cast<Detail::PluginMemory*>(user);
    memory->allocated += size;

    // ...
    return block;
}

void Djinn::Base::FreeCallback(void* user, void *block) {
    // Get size of block
    USize size;
#ifdef DJINN_PLATFORM_WINDOWS
    size = _msize(block);
#else
    size = malloc_usable_size(block);
#endif

    // Profile
    auto memory = static_cast<Detail::PluginMemory*>(user);
    memory->allocated -= size;

    // ...
    free(block);
}
