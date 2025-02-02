//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Reflection/ReflectionRegistry.h>
#include <Base/DefaultLogs.h>
#include <Reflection/PluginAPI.h>

DJINN_NS2(Reflection);

namespace {

}

DJINN_C_EXPORT_PLUGINS_REFLECTION COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Install reflection registry
    registry->InstallClass<ReflectionRegistry>();

    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        ReflectionRegistry *reflectionRegistry;
        if (!registry->CreateClass(plugin, &reflectionRegistry) || !registry->GetPipeline()->AddInterface(reflectionRegistry)) {
            ErrorLog(registry).Write("Failed to install reflection registry");
            return COM::kError;
        }

        /// Reflector used in plugin construction
        struct Reflector : public IReflector {
            ReflectionRegistry *registry;

            /// Override
            void AddClass(IClassInfo *info) override {
                if (COM::CheckedResult result = registry->AddClass(info)) {
                    ErrorLog(registry).Write("Failed to install reflection information");
                }
            }

            /// Override
            void AddEnum(IEnumInfo *info) override {
                if (COM::CheckedResult result = registry->AddEnum(info)) {
                    ErrorLog(registry).Write("Failed to install reflection information");
                }
            }
        };

        // ...
        Reflector reflector;
        reflector.registry = reflectionRegistry;

        // Register plugin hook
        registry->BindPluginResolved([=](const Platform::Library &library) mutable {
            if (auto addr = library.GetNamedAddress<Reflection::PluginAPI::Reflect>(DJINN_STRINGIFY(DJINN_PLUGINAPI_REFLECT))) {
                // Reflect plugin
                addr(registry, &reflector);

                // ...
                InfoLog(registry).Write("Reflected plugin '", library.GetName(), "'");
            }
        });

        // Current reflected code
        if (auto addr = Platform::LibraryAPI::GetNamedAddress<Reflection::PluginAPI::Reflect>(Platform::LibraryAPI::GetCurrent(), DJINN_STRINGIFY(DJINN_PLUGINAPI_REFLECT))) {
            // Reflect source
            addr(registry, &reflector);

            // ...
            InfoLog(registry).Write("Reflected executable");
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_REFLECTION COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    Check(registry->GetPipeline()->RemoveInterface(IReflectionRegistry::kIID));

    return COM::kOK;
}
