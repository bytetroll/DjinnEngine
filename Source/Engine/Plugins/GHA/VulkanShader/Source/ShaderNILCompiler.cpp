#include <GHA/Vulkan/ShaderNILCompiler.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Stage.h>
#include <GHA/Vulkan/GlassManager.h>
#include <GHA/Vulkan/GlslangShader.h>
#include <GHA/Vulkan/GlassCompiler.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/Program.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <Platform/FileSystem.h>
#include <Platform/FileStream.h>

DJINN_NS3(GHA, Vulkan);

ShaderNILCompiler::ShaderNILCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IShaderNILCompiler(registry, outer, host) {

}

COM::Result ShaderNILCompiler::Initialize() {
    // OK
    return COM::kOK;
}

COM::Result ShaderNILCompiler::Compile(NyLang::Registry* nyRegistry, const Data::ShaderCompileNILDesc &desc, NyLang::IL::Program *program, GHA::ShaderBlob* shaderBlob, ICompiledShaderBlob** out) {
    /*NyLang::PrettyStream stream;
    program->PrettyPrint(stream);
    Platform::FileStream fs(Platform::FileMode::eWrite, "debug.nil");
    fs.Write(stream.GetData().c_str(), stream.GetData().size());
    fs.Close();*/

    // Create shader blob
    Vulkan::CompiledShaderBlob* blob;
    if (!registry->CreateClass(nullptr, &blob)) {
        ErrorLog(this).Write("Failed to create a vulkan shader blob");
        return COM::kError;
    }

    // Initialize glass manager
    GlassManager glassManager(registry, desc);

    // Compile manager
    GlassCompiler compiler(registry, &glassManager);
    if (!compiler.Compile(nyRegistry, desc, program, shaderBlob, blob)) {
        return COM::kError;
    }

    // Compile to glsl
    Core::String glsl;
    if (!glassManager.Compile(glsl)) {
        return COM::kError;
    }

    // Initialize glsl shader
    GlslangShader glslangShader(registry, static_cast<EShLanguage>(glassManager.getStage()));

    // Compile to spirv
    if (!glslangShader.CompileSPIRV(glsl, glassManager.getVersion(), blob)) {
        return COM::kError;
    }

    // OK
    *out = blob;
    return COM::kOK;
}

