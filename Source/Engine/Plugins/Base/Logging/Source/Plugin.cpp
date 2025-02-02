//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/ConsoleLogStream.h>
#include <Base/DefaultLogs.h>
#include <Core/CrashHandler.h>

DJINN_NS2(Base);

DJINN_C_EXPORT_PLUGINS_BASE_LOGGING COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry) {
    if (COM::CheckedResult result = registry->GetPipeline()->AddClass(new ConsoleLogStream(registry, nullptr, nullptr))) {
        return result;
    }

    // Install crash handler
    InstallCrashHandler();

    // Hook installed
    registry->BindPluginInstalled([=](const Core::StringView& module, double elapsed) {
        InfoLog(registry).Write("Loaded plugin: '", module, "' - ", elapsed, "s");
    });

    // Hook destroyed
    registry->BindPluginDestroyed([=](const Core::StringView& module, double elapsed) {
        InfoLog(registry).Write("Unloaded plugin: '", module, "' - ", elapsed, "s");
    });

    // Hook resolving
    registry->BindPluginResolve([=](UInt32 count) {
        InfoLog(registry).Write("Loading and resolving ", count, " plugin(s)");
    });

    // Hook failed
    registry->BindPluginFailed([=](const Core::StringView& module, const Core::StringView& nativeError, Host::LoadError error) {
        const char *reason = nullptr;
        switch (error) {
            case Host::LoadError::eBadModule: {
                reason = "bad module (Invalid path or incompatible dll with possible missing dependencies)";
                break;
            }
            case Host::LoadError::eInstallFailed: {
                reason = "installation failed";
                break;
            }
            case Host::LoadError::eDestroyFailed: {
                reason = "destruction failed";
                break;
            }
            case Host::LoadError::eFailedResolve: {
                reason = "failed to resolve load order";
                break;
            }
            case Host::LoadError::eNoInstallAPI: {
                reason = "no install callback, each plugin must implement the install and destroy callback";
                break;
            }
            case Host::LoadError::eNoDestroyAPI: {
                reason = "no destroy callback, each plugin must implement the install and destroy callback";
                break;
            }
            case Host::LoadError::eNoInfoAPI: {
                reason = "no info callback, each plugin must implement the info callback";
                break;
            }
        }
        if (nativeError.Ptr()) {
            InfoLog(registry).Write("Failed to load plugin: '", module, "', ", reason, ", native message:", "\n\t", nativeError);
        } else {
            InfoLog(registry).Write("Failed to load plugin: '", module, "', ", reason);
        }
    });

    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_BASE_LOGGING COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    Check(registry->GetPipeline()->RemoveClass(ConsoleLogStream::kCID));

    return COM::kOK;
}
