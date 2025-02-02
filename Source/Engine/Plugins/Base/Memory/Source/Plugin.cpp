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
 on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Core/CrashHandler.h>
#include <Core/Atomic.h>
#include <Platform/LibraryMemoryAPI.h>
#include <Base/MemoryCallbacks.h>
#include <Base/Detail/PluginMemory.h>

DJINN_NS2(Base);

DJINN_C_EXPORT_PLUGINS_BASE_MEMORY COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry) {
    // Hook resolving
    registry->BindPluginResolved([=](const Platform::Library &library) {
        auto memory = new Detail::PluginMemory();
        memory->library = library;
        memory->allocated = 0;

        // Assign malloc
        if (auto hook = library.GetHook<Platform::LibraryMemoryAPI::MallocHook>()) {
            hook.Assign(memory, MallocCallback);
        }

        // Assign free
        if (auto hook = library.GetHook<Platform::LibraryMemoryAPI::FreeHook>()) {
            hook.Assign(memory, FreeCallback);
        }

        // ...
        Detail::pluginMemory.Add(memory);
    });

    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_BASE_MEMORY COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
