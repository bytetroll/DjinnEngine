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
