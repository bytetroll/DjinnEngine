//
// Created by Dev on 12/26/2017.
//
#pragma once

namespace Djinn::Graphics {
    enum class DependencyMode {
        /// Implicit dependency mode, all dependencies will be resolved from the recorded commands
        eImplicit,

        /// Explicit dependency mode, uses the supplied dependencies
        eExplicit
    };
}