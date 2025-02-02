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
