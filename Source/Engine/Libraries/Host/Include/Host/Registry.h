#pragma once

#include "IRegistry.h"
#include "COM/IInstanceHost.h"
#include "InstanceHost.h"
#include "Pipeline.h"
#include <Core/String.h>
#include <Platform/LibraryAPI.h>
#include <Core/Map.h>
#include <Platform/Library.h>
#include <Core/Array.h>
#include "PluginInfo.h"

namespace Djinn {
    namespace Host {
        class Plugin;

        class Registry : public IRegistry {
        public:
            DJINN_COM_CLASSID();

            Registry();
            ~Registry();

            COM::Result LoadPlugins(const Core::StringView &pattern, LoadFlagSet flags) override;
            COM::Result UnloadPlugin(const Core::StringView &name) override;
            COM::Result UnloadPlugins() override;
            COM::Result LoadPlugin(const Core::StringView &name, LoadFlagSet flags) override;
            void InstallClass(const COM::ClassID &id, const COM::InterfaceID &iid, COM::IInstanceHost *host, bool inheritHost) override;
            void RemoveClass(const COM::ClassID &id) override;
            COM::Result CreateClass(const COM::ClassID &id, COM::IUnknown *outer, const COM::InterfaceID &iid, void **out) override;
            IPipeline *GetPipeline() override;
            COM::Result GetClasses(const COM::InterfaceID &id, USize &count, COM::ClassID *out) override;
            bool HasClass(const COM::ClassID &id) override;
            void BindPluginInstalled(const PluginInstalledDelegate &delegate) override;
            void BindPluginDestroyed(const PluginDestroyedDelegate &delegate) override;
            void BindPluginFailed(const PluginFailedDelegate &delegate) override;
            void BindPluginResolve(const PluginResolveDelegate &delegate) override;
            void BindPluginResolved(const PluginResolvedDelegate &delegate) override;

        private:
            COM::Result PreloadPlugin(const Core::StringView &path);
            COM::Result DestroyPlugin(const Core::StringView &path);

            Core::String GetModulePath(const Core::StringView &name);

            struct PluginState {
                Plugin *com = nullptr;
                PluginInfo info;
                Platform::Library library;
                bool loaded = false;
            };

            Pipeline pipeline;
            Core::HashMap<Core::String, Core::String> modules;
            Core::HashMap<Core::String, PluginState> libraries;
            Core::Array<Core::String> order;
            Core::HashMap<COM::ClassID, COM::IInstanceHost *> classes;
            Core::Array<COM::IInstanceHost *> inheritedHosts;
            Core::HashMap<COM::InterfaceID, Core::Array<COM::ClassID>> interfaces;
            Core::Array<PluginInstalledDelegate> pluginInstalledActions;
            Core::Array<PluginInstalledDelegate> pluginDestroyedActions;
            Core::Array<PluginResolvedDelegate> pluginResolvedActions;
            Core::Array<PluginFailedDelegate> pluginFailedActions;
            Core::Array<PluginResolveDelegate> pluginResolveActions;
        };
    }
}