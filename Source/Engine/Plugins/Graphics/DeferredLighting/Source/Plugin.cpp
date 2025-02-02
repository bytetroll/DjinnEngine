//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/CameraProcess.h>
#include <Graphics/IWorldProcess.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPipeline.h>
#include <Game/IWorldHost.h>
#include <Graphics/DeferredLightingWorldPackage.h>
#include <Graphics/DeferredGBufferPass.h>
#include <Data/IMaterialCompiler.h>
#include <Graphics/DeferredGBufferMaterialType.h>
#include <Graphics/DeferredDirectionalLightPass.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Camera plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<Graphics::DeferredLightingWorldPackage>();

        Data::IMaterialCompiler* materialCompiler;
        if (!registry->GetPipeline()->GetInterface(&materialCompiler)) {
            ErrorLog(registry).Write("Failed to install plugin, expected material compiler to be installed");
            return COM::kError;
        }

        Graphics::DeferredGBufferMaterialType* gbufferType;
        if (!registry->CreateClass(plugin, &gbufferType) || !materialCompiler->Register(gbufferType)) {
            ErrorLog(registry).Write("Failed to install gbuffer material type");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
