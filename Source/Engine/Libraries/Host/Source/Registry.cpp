#include <Host/PluginAPI.h>
#include <Core/Memory.h>
#include <Core/StringStream.h>
#include <Core/Timer.h>
#include <Platform/FileSystem.h>
#include <Platform/FileStream.h>
#include <Host/Registry.h>
#include <Host/Plugin.h>
#include <iostream>

DJINN_NS2(Host);

Registry::Registry() {
    IRegistry::InstallClass<Plugin>();
}

Registry::~Registry() {
    // Unload all installed plugins
    if (!UnloadPlugins()) {
        std::cout << "Failed to unload one or more registry plugins, live plugins:\n";
        for (auto& name : order) {
            std::cout << "\t" << name.AsCharArray() << "\n";
        }
        DJINN_ASSERT(false, "One or more plugins failed to unload");
    }

    // Delete all hosts
    for (auto host : inheritedHosts) {
        delete host;
    }
}

COM::Result Registry::LoadPlugin(const Core::StringView& name, LoadFlagSet flags) {
    // To module path
    auto modulePath = GetModulePath(name);

    // Loaded?
    if (!libraries.Contains(modulePath)) {
        if (COM::CheckedResult result = PreloadPlugin(modulePath)) {
            return result;
        }
    }

    // Get plugin
    auto &plugin = libraries[modulePath];

    // Not loaded?
    if (!plugin.loaded) {
        // Get install address
        auto Install = plugin.library.GetNamedAddress<PluginAPI::Install>(DJINN_STRINGIFY(DJINN_PLUGINAPI_INSTALL));
        if (!Install) {
            pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), nullptr, LoadError::eNoInstallAPI); });
            return kNoPluginInstallAddr;
        }

        // Attempt to install
        Core::Timer timer;
        if ((*Install)(this, flags, plugin.com) == COM::kOK) {
            double delta = timer.Delta();
            pluginInstalledActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), delta); });
        } else {
            pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), nullptr, LoadError::eInstallFailed); });
            return kBadPluginInstall;
        }

        // ...
        order.Add(modulePath);

        // Mark
        plugin.loaded = true;
    }

    // OK
    return COM::kOK;
}

void Registry::InstallClass(const COM::ClassID &id, const COM::InterfaceID &iid, COM::IInstanceHost *host, bool inheritHost) {
    // ...
    if (iid) {
        interfaces[iid].Add(id);
    }

    // Insert
    classes.Add(id, host);

    // Inherit?
    if (inheritHost) {
        inheritedHosts.Add(host);
    }
}

void Registry::RemoveClass(const COM::ClassID &id) {
    // Must exist
    auto it = classes.Find(id);
    if (it == classes.End()) {
        return;
    }

    // Erase
    classes.Remove(it);
}

COM::Result Registry::CreateClass(const COM::ClassID &id, COM::IUnknown *outer, const COM::InterfaceID &iid, void **out) {
    // Must exist
    auto it = classes.Find(id);
    if (it == classes.End()) {
        return kUnknownClass;
    }

    // TODO: ...
    COM::IUnknown *instance;
    if ((it.handle->second->CreateInstance(this, outer, &instance)) != COM::kOK) {
        return COM::kInvalidParameter;
    }

    // Query for interface
    return instance->QueryInterface(iid, out);
}

IPipeline *Registry::GetPipeline() {
    return &pipeline;
}

COM::Result Registry::GetClasses(const COM::InterfaceID &id, USize &count, COM::ClassID *out) {
    auto &ids = interfaces[id];

    // ...
    count = ids.Size();

    // If out
    if (out) {
        Core::Memory::Copy(out, ids.Ptr(), sizeof(COM::ClassID) * count);
    }

    // OK
    return COM::kOK;
}

bool Registry::HasClass(const COM::ClassID &id) {
    return classes.Contains(id);
}

void Registry::BindPluginInstalled(const PluginInstalledDelegate &delegate) {
    pluginInstalledActions.Add(delegate);
}

void Registry::BindPluginDestroyed(const PluginDestroyedDelegate &delegate) {
    pluginDestroyedActions.Add(delegate);
}

void Registry::BindPluginFailed(const PluginFailedDelegate &delegate) {
    pluginFailedActions.Add(delegate);
}

COM::Result Registry::LoadPlugins(const Core::StringView& pattern, LoadFlagSet flags) {
    // Get all dlls
    Core::Array<Core::String> files;
    if (!Platform::FileSystem::Glob(files, pattern)) {
        return COM::kError;
    }

    // ...
    pluginResolveActions.Foreach([&](auto delegate) { delegate.Invoke(static_cast<UInt32>(files.Size())); });

    // Precache all plugins
    for (auto &file : files) {
        if (COM::CheckedResult result = PreloadPlugin(file.AsCharArray())) {
            return result;
        }
    }

    // Retrofit dependencies
    for (auto &file : files) {
        auto &plugin = libraries[file];
        for (auto &dep : plugin.info.dependencies) {
            if (dep.mode == PluginDependencyMode::eBefore) {
                auto &dependent = libraries[GetModulePath(dep.name)];

                // Must inherit all dependencies
                plugin.info.dependencies.Add(dependent.info.dependencies);

                dependent.info.dependencies.Add({plugin.info.name.AsCharArray(), PluginDependencyMode::eAfter});
            }
        }
    }

    /*{
        Platform::FileStream stream(Platform::FileMode::eWrite, "dependencies.short.graph");

        Core::StringStream ss(&stream);
        ss << "digraph IncludeGraph {\n";

        for (auto &file : files) {
            auto &plugin = libraries[file];
            ss << "\t" << plugin.info.name.Hash() << " [label=\"" << plugin.info.name << "\"];\n";
        }

        for (auto &file : files) {
            auto &plugin = libraries[file];
            for (auto &dep : plugin.info.dependencies) {
                if (dep.mode == PluginDependencyMode::eBefore) {
                    continue;
                }
                ss << "\t" << plugin.info.name.Hash() << " -> " << Core::ComputeCRC64(dep.name) << "; #" << dep.name << "\n";
            }
        }

        ss << "}\n";
        stream.Close();
    }*/

    // Attempt to resolve
    bool mutated;
    do {
        mutated = false;
        for (Int64 i = static_cast<Int64>(files.Size() - 1); i >= 0; i--) {
            auto &plugin = libraries[files[i]];
            if (plugin.info.dependencies.All([&](const PluginDependency &dep) {
                if (dep.mode == PluginDependencyMode::eBefore) {
                    return true;
                }
                return libraries[GetModulePath(dep.name)].loaded;
            })) {
                if (COM::CheckedResult result = LoadPlugin(plugin.info.name.AsCharArray(), flags)) {
                    return result;
                }
                files.Remove(static_cast<UInt32>(i));
                mutated = true;
            }
        }
    } while (mutated);

    // Must be 0
    if (files.Size() != 0) {
        for (auto& file : files) {
            pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(file.AsCharArray(), nullptr, LoadError::eFailedResolve); });
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result Registry::PreloadPlugin(const Core::StringView& path) {
    if (!libraries.Contains(path.ToString())) {
        PluginState plugin;

        // Create worldHost
        if (COM::CheckedResult result = IRegistry::CreateClass(nullptr, &plugin.com)) {
            return result;
        }

        // Note: In some occasions loading of libraries require absolute paths
        Core::String modulePath = path.ToString();

        // Get directory path
        Core::String directory;
        if (Platform::FileSystem::GetExecutableDirectory(directory)) {
            modulePath = Platform::FileSystem::CombinePaths(directory, modulePath);
        }

        // Attempt to load
        const char* error;
        if (!plugin.library.Load(modulePath.AsCharArray(), &error)) {
            pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(path, error, LoadError::eBadModule); });
            return COM::kInvalidParameter;
        }

        // Resolved
        pluginResolvedActions.Foreach([&](auto delegate) { delegate.Invoke(plugin.library); });

        // Get info address
        auto Info = plugin.library.GetNamedAddress<PluginAPI::Info>(DJINN_STRINGIFY(DJINN_PLUGINAPI_INFO));
        if (!Info) {
            pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(path, nullptr, LoadError::eNoInfoAPI); });
            return kNoPluginInfoAddr;
        }

        // Pool
        if (COM::CheckedResult result = Info(plugin.info)) {
            return result;
        }

        // ...
        libraries[path.ToString()] = plugin;
    }

    // OK
    return COM::kOK;
}

void Registry::BindPluginResolve(const PluginResolveDelegate &delegate) {
    pluginResolveActions.Add(delegate);
}

void Registry::BindPluginResolved(const PluginResolvedDelegate &delegate) {
    pluginResolvedActions.Add(delegate);
}

COM::Result Registry::UnloadPlugin(const Core::StringView& name) {
    return DestroyPlugin(GetModulePath(name));
}

COM::Result Registry::UnloadPlugins() {
    while (order.Size()) {
        // Always destroy last first
        if (COM::CheckedResult result = DestroyPlugin(order.Last())) {
            return result;
        }

        // ...
        order.PopEnd();
    }

    // OK
    return COM::kOK;
}

Core::String Registry::GetModulePath(const Core::StringView &name) {
    return name.ToString() + ".dll";
}

COM::Result Registry::DestroyPlugin(const Core::StringView &path) {
    auto modulePath = path.ToString();

    // Must be a valid library
    if (!libraries.Contains(modulePath)) {
        return kPluginNotLoaded;
    }

    auto& lib = libraries[modulePath];

    // Get install address
    auto Destroy = lib.library.GetNamedAddress<PluginAPI::Destroy>(DJINN_STRINGIFY(DJINN_PLUGINAPI_DESTROY));
    if (!Destroy) {
        pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), nullptr, LoadError::eNoDestroyAPI); });
        return kNoPluginInstallAddr;
    }

    // Attempt to destroy
    Core::Timer timer;
    if ((*Destroy)(this, lib.com) == COM::kOK) {
        double delta = timer.Delta();
        pluginDestroyedActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), delta); });
    } else {
        pluginFailedActions.Foreach([&](auto delegate) { delegate.Invoke(modulePath.AsCharArray(), nullptr, LoadError::eDestroyFailed); });
        return kBadPluginInstall;
    }

    // Release the component
    if (COM::CheckedResult result = lib.com->Release()) {
        return result;
    }

    // Mark as unloaded
    lib.loaded = false;

    // OK
    return COM::kOK;
}
