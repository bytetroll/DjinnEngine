#pragma once

#include <COM/Result.h>
#include <Core/BitSet.h>
#include <Core/Template/Limits.h>
#include "PluginInfo.h"
#include "IPlugin.h"

namespace Djinn {
    namespace Host {
        class IRegistry;

        enum class LoadFlag {
            // Basic registry like installs
                    eMinimal,

            // With pipeline integration
                    ePipeline = 1,

            // All flags
                    eAll = Core::LimitsOf<int>::kMax,
        }; DJINN_CORE_BITSET(LoadFlag);

        enum class LoadError {
            eBadModule,
            eNoInfoAPI,
            eNoInstallAPI,
            eNoDestroyAPI,
            eFailedResolve,
            eInstallFailed,
            eDestroyFailed
        };

        namespace PluginAPI {
            // Plugin info callback
#define DJINN_PLUGINAPI_INFO Info
            using Info = COM::Result(*)(PluginInfo& out);

            // Plugin install callback
#define DJINN_PLUGINAPI_INSTALL Install
            using Install = COM::Result(*)(IRegistry *, LoadFlagSet, IPlugin*);

            // Plugin destroy callback
#define DJINN_PLUGINAPI_DESTROY Destroy
            using Destroy = COM::Result(*)(IRegistry *, IPlugin*);
        }
    }
}