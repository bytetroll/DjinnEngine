//
// Created by Dev on 1/4/2018.
//
#pragma once

#include <Async/ContextAPI.h>
#include <Async/ContextData.h>

namespace Djinn::Async {
    class Framework;
}

namespace Djinn::Async::Detail {
    /// Framework fetch helper
    static Framework* GetFramework() {
        return reinterpret_cast<Framework*>(reinterpret_cast<ContextData*>(API::GetContextData())->owner);
    }
}
