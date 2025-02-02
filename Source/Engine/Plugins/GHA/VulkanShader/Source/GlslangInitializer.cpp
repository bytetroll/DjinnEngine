//
// Created by Dev on 11/30/2017.
//

#include <glslang/ShaderLang.h>

/// Glslang global initialization helper
struct GLSLangThreading {
    GLSLangThreading() {
        glslang::InitializeProcess();
    }

    ~GLSLangThreading() {
        glslang::FinalizeProcess();
    }
} _gGLSLangThreaded;