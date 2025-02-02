//
// Created by Dev on 1/31/2018.
//
#pragma once

namespace Djinn::Data {
    enum class TextureCompileMode {
        /// The compilation will be launched on a single job
        eInline,

        /// All cores will be utilized, not suitable for responsiveness
        eExternal
    };
}