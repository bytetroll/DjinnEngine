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
 on 2/25/2018.
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