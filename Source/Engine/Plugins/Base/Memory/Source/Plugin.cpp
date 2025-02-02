//
// Created by Dev on 8/16/2017.
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
