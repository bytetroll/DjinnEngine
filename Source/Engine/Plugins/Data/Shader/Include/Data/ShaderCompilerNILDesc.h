//
// Created by Dev on 12/9/2017.
//
#pragma once

#include "ShaderConfiguration.h"
#include <GHA/Stage.h>

namespace Djinn {
    namespace Data {
        struct ShaderCompileNILDesc {
            // Description
            const ShaderConfiguration* desc = nullptr;

            // Target entry point name
            Core::String entryPoint;

            // Targeted stage
            GHA::Stage stage;
        };
    }
}