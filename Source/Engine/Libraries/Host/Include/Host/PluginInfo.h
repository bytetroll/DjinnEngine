//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Array.h>
#include <Core/String.h>

namespace Djinn::Host {
    enum class PluginDependencyMode {
        eBefore,
        eAfter
    };

    struct PluginDependency {
        const char *name = "";
        PluginDependencyMode mode;
    };

    struct PluginInfo {
        PluginInfo &AddDependency(PluginDependencyMode mode, const char *name) {
            dependencies.Add({name, mode});
            return *this;
        }

        PluginInfo &Description(const Core::String &value) {
            description = value;
            return *this;
        }

        PluginInfo &Name(const Core::String &value) {
            name = value;
            return *this;
        }

        Core::String name;

        Core::Array<PluginDependency> dependencies;

        Core::String description;
    };
}