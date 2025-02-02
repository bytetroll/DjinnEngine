//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/WorldProcess.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IGraph.h>
#include <Base/DefaultLogs.h>
#include <Graphics/WorldPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/WorldPackage.h>
#include <Game/IWorldHost.h>
#include <Graphics/PacketSystem.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_WORLD COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("World plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<Graphics::WorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_WORLD COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
