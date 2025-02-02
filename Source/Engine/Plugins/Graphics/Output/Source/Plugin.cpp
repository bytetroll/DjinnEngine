//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IGraph.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPipeline.h>
#include <Graphics/Output.h>
#include <Graphics/OutputPass.h>
#include <Graphics/OutputHost.h>
#include <Graphics/IProcess.h>
#include <Graphics/ScreenOutput.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_OUTPUT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get graphics system
        Graphics::IGraphicsSystem *system;
        if (!registry->GetPipeline()->GetInterface(&system)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, expected graphics system to be installed");
            return COM::kError;
        }

        // Get process
        auto process = system->GetGraph()->GetProcess();

        // Install output pass
        Graphics::OutputPass *pass;
        if (!registry->CreateClass(plugin, &pass) || !pass->Initialize() || !process->GetPipeline()->Register(pass)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, failed to install output pass");
            return COM::kError;
        }

        // Install output worldHost
        Graphics::OutputHost* host;
        if (!registry->CreateClass(plugin, &host) || !host->Initialize(pass) || !registry->GetPipeline()->AddInterface(host)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, failed to install output worldHost");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_OUTPUT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
