//
// Created by Dev on 6/27/2018.
//

#include <Base/IWindowSystem.h>
#include <Host/PluginAPI.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS();

DJINN_C_EXPORT_EDITOR_WINDOW COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Get window
    Base::IWindowSystem* window;
    if (!registry->GetPipeline()->GetInterface(&window)) {
        ErrorLog(registry).Write("Editor window plugin expects an installed window system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_WINDOW COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
