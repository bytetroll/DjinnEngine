//
// Created by Dev on 5/13/2018.
//
#pragma once

#include <Core/Atomic.h>
#include <Core/Array.h>
#include <Platform/Library.h>

namespace Djinn::Base::Detail {
    struct PluginMemory {
        Platform::Library library;
        Core::AtomicCounter allocated;
    };

    extern Core::Array<PluginMemory*> pluginMemory;
}