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
 on 11/30/2017.
//

#include <GHA/Vulkan/GlslangShader.h>
#include <glslang/ShHandle.h>
#include <glslang/localintermediate.h>
#include <Core/Common.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <SPIRV/GlslangToSpv.h>
#include <GHA/ShaderBlob.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>

DJINN_NS3(GHA, Vulkan);

GlslangShader::GlslangShader(Host::IRegistry *registry, EShLanguage Lang) : glslang::TShader(Lang), registry(registry) {

}

GlslangShader::~GlslangShader() {

}

COM::Result GlslangShader::CompileSPIRV(const Core::String &glsl, int version, Vulkan::CompiledShaderBlob* blob) {
    //std::cout << glsl;

    // Set program
    const char *programs[] = {glsl.AsCharArray()};
    setStrings(programs, 1);

    // Entry point always compiled to main
    setEntryPoint("main");

    // Parse shader
    if (!parse(&limits, version, false, static_cast<EShMessages >(EShMsgSpvRules | EShMsgVulkanRules))) {
        ErrorLog(registry).Write("Glslang shader compilation failed, messages:\n", compiler->infoSink.info.c_str(), "\n", compiler->infoSink.debug.c_str());
        ErrorLog(registry).Write("Full broken glslang shader:\n", glsl);
        return COM::kError;
    }

    // Ensure intermediate
    TInfoSink sink;
    intermediate->finalCheck(sink, false);
    if (intermediate->getNumErrors()) {
        ErrorLog(registry).Write("Glslang shader compilation failed");
        return COM::kError;
    }

    // Translate to spirv
    spv::SpvBuildLogger logger;
    glslang::GlslangToSpv(*intermediate, blob->blob.handle, &logger);
    if (logger.getAllMessages().size()) {
        ErrorLog(registry).Write("Glslang shader compilation spirv translation failed, messages:\n", static_cast<int>(logger.getAllMessages().size()));
        return COM::kError;
    }

    // ...
    return COM::kOK;
}
