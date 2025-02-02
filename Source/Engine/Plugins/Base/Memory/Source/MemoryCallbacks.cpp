//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 5/13/2018.
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
