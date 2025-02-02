//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Async/Worker.h>
#include <Async/Scheduler.h>
#include <Async/Framework.h>
#include <Platform/User.h>

DJINN_NS2(Async);

DJINN_C_EXPORT_PLUGINS_ASYNC COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        // Initialize scheduler
        Scheduler *scheduler;
        if (!registry->CreateClass(plugin, &scheduler) || !scheduler->Initialize()) {
            ErrorLog(registry).Write("Failed to install plugin, could not initialize async scheduler");
            return COM::kError;
        }

        // Initialize default lanes
        if (!scheduler->CreateLane(Async::kDefault, Math::Max(1U, Platform::User::GetCoreCount())) ||
            !scheduler->CreateLane(Async::kResource, Math::Max(1U, Platform::User::GetCoreCount()))) {
            ErrorLog(registry).Write("Failed to install plugin, could not create default async lanes");
            return COM::kError;
        }

        // Initialize framework
        Framework *framework;
        if (!registry->CreateClass(plugin, &framework) || !framework->Initialize(scheduler)) {
            ErrorLog(registry).Write("Failed to install plugin, could not initialize async framework");
            return COM::kError;
        }

        // Pipeline
        Check(registry->GetPipeline()->AddInterface(scheduler));
        Check(registry->GetPipeline()->AddClass(framework));
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_ASYNC COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
