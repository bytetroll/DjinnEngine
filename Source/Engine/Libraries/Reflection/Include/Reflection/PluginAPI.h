#pragma once

#include "IReflector.h"

namespace Djinn::Host {
    class IRegistry;
}

namespace Djinn::Reflection {
    namespace PluginAPI {
        // Plugin info callback
#define DJINN_PLUGINAPI_REFLECT Reflect
        using Reflect = void (*)(Host::IRegistry *registry, IReflector* reflector);
    }
}