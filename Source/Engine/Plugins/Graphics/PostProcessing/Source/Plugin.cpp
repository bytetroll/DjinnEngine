//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/PostProcessingProcess.h>
#include <Base/DefaultLogs.h>
#include <Graphics/PostProcessingWorldPackage.h>
#include <Game/IWorldHost.h>
#include <Graphics/PostProcessingPass.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_POSTPROCESSING COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Post processing plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<Graphics::PostProcessingWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_POSTPROCESSING COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
