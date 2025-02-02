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
 on 9/4/2017.
//

#include <Data/ShaderCompiler.h>
#include <Data/IShaderNILCompiler.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Data/ShaderAsset.h>
#include <NyLang/Core/Host/Registry.h>
#include <NyLang/Diagnostic/Diagnostic.h>
#include <NyLang/Frontend/Compiler/Compiler.h>
#include <NyLang/Frontend/Compiler/CompilerContext.h>
#include <NyLang/Frontend/Semantic/SemanticContext.h>
#include <NyLang/Frontend/Parser/Parser.h>
#include <NyLang/Frontend/Tokenizer/Tokenizer.h>
#include <NyLang/Frontend/Semantic/Semantic.h>
#include <NyLang/Backend/IL/Program.h>
#include <NyLang/Backend/IL/Container.h>
#include <NyLang/Backend/IL/Function.h>
#include <NyLang/Backend/IL/IType.h>
#include <NyLang/Backend/IL/Argument.h>
#include <NyLang/Native/NativeExtensions.h>
#include <Data/NyShader/Operations.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <NyLang/Backend/Pass/PassManager.h>
#include <NyLang/Native/Passes/DeadIRPass.h>
#include <NyLang/Native/IL/Values/ConstantMetaValue.h>
#include <NyLang/Native/IL/Values/ConstantStringValue.h>
#include <NyLang/Native/IL/Types/ClassType.h>
#include <NyLang/Native/IL/Class.h>
#include <NyLang/Native/IL/Values/ConstantIntValue.h>
#include <Data/NyShader/NILTypes.h>
#include <GHA/Format.h>
#include <NyLang/Native/IL/Types/FPType.h>
#include <NyLang/Native/IL/Types/IntegerType.h>
#include <NyLang/Frontend/Symbol/SymbolTree.h>
#include <Core/StringStream.h>
#include <Core/Timer.h>
#include <Data/ShaderCompilerPool.h>
#include <Core/Set.h>
#include <Platform/LockGuard.h>

DJINN_NS2(Data);

ShaderCompiler::ShaderCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IShaderCompiler(registry, outer, host) {

}

COM::Result ShaderCompiler::Initialize() {
    if (!registry->CreateClass(this, &defaultPool) || !defaultPool->Initialize()) {
        ErrorLog(this).Write("Failed to create shader compiler pool");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result ShaderCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    // Pool registry
    NyLang::Registry* registry;
    if (!defaultPool->Pool(&registry)) {
        return COM::kError;
    }

    if (!Compile(registry, stream, _config, out)) {
        return COM::kError;
    }

    // Done
    defaultPool->Push(registry);
    return COM::kOK;
}

COM::Result ShaderCompiler::AddNILCompiler(IShaderNILCompiler *compiler) {
    compilers.Add(compiler->GetClassID(), compiler);
    return COM::kOK;
}

COM::Result ShaderCompiler::RemoveNILCompiler(const COM::ClassID &cid) {
    return compilers.Remove(cid) ? COM::kOK : kNoNILCompiler;
}

COM::Result ShaderCompiler::FillVertexAttributes(GHA::InputAssemblyDesc &desc, NyLang::IL::IValue *value, NyShader::AnnotationChain chain) {
    auto type = value->GetType();
    for (auto &annot : value->GetAnnotations()) {
        if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
            if (meta->GetName() == "Attribute") {
                chain.Add(meta);
            }
        }
    }
    if (auto _class = NyLang::OuterCast<NyLang::IL::ClassType>(type)) {
        for (auto &member : _class->GetContained()->GetMembers()) {
            if (!FillVertexAttributes(desc, member, chain)) {
                return COM::kError;
            }
        }
    } else {
        if (chain.index > 0) {
            auto meta = chain.annotations[chain.index - 1];

            NyLang::String name = NyLang::OuterCast<NyLang::IL::ConstantStringValue>(meta->GetMemberWithName("name"))->GetString();
            Int64 offset = NyLang::OuterCast<NyLang::IL::ConstantIntValue>(meta->GetMemberWithName("offset"))->GetValue();

            desc.inputs.Resize(static_cast<USize>(offset + 1));

            USize stride = 0;

            GHA::InputAttributeDesc attributeDesc;
            attributeDesc.inputIndex = static_cast<UInt32>(offset);

            switch (type->GetClassID()) {
                case Data::NyShader::VectorType::kCID: {
                    auto vectorType = static_cast<Data::NyShader::VectorType *>(type);
                    switch (vectorType->type->GetClassID()) {
                        case NyLang::IL::FPType::kCID: {
                            switch (vectorType->x) {
                                case 1:
                                    attributeDesc.format = GHA::Format::eR32F;
                                    stride = 4;
                                    break;
                                case 2:
                                    attributeDesc.format = GHA::Format::eR32G32F;
                                    stride = 8;
                                    break;
                                case 3:
                                    attributeDesc.format = GHA::Format::eR32G32B32F;
                                    stride = 12;
                                    break;
                                case 4:
                                    attributeDesc.format = GHA::Format::eR32G32B32A32F;
                                    stride = 16;
                                    break;
                                default:
                                    ErrorLog(this).Write("Invalid base vector type\n");
                                    return COM::kError;
                            }
                            break;
                        }
                        case NyLang::IL::IntegerType::kCID: {
                            switch (vectorType->x) {
                                case 1:
                                    attributeDesc.format = GHA::Format::eR32U;
                                    stride = 4;
                                    break;
                                case 2:
                                    attributeDesc.format = GHA::Format::eR32G32U;
                                    stride = 8;
                                    break;
                                case 3:
                                    attributeDesc.format = GHA::Format::eR32G32B32U;
                                    stride = 12;
                                    break;
                                case 4:
                                    attributeDesc.format = GHA::Format::eR32G32B32A32U;
                                    stride = 16;
                                    break;
                                default:
                                    ErrorLog(this).Write("Invalid base vector type\n");
                                    return COM::kError;
                            }
                            break;
                        }
                    }
                    break;
                }
                case NyLang::IL::FPType::kCID: {
                    attributeDesc.format = GHA::Format::eR32F;
                    stride = 4;
                    break;
                }
                case NyLang::IL::IntegerType::kCID: {
                    attributeDesc.format = GHA::Format::eR32I;
                    stride = 4;
                    break;
                }
                default:
                    ErrorLog(this).Write("NInvalid base input type\n");
                    return COM::kError;
            }
            desc.inputAttributes.Add(attributeDesc);

            desc.inputs[offset].stride += stride;

            if (name == "vertex") {
                desc.inputs[offset].rate = GHA::InputRate::eVertex;
            } else if (name == "instance") {
                desc.inputs[offset].rate = GHA::InputRate::eInstance;
            } else {
                ErrorLog(this).Write("Attribute name invalid\n");
                return COM::kError;
            }
        }
    }
    return COM::kOK;
}

COM::Result ShaderCompiler::Compile(NyLang::Registry* registry, Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = static_cast<ShaderConfiguration *>(_config);

    // Diagnostic
    Core::Timer timer;
    InfoLog(this).Write("Started shader compilation job [", config->Hash(), "]");

    // Create asset
    ShaderAsset *asset;
    if (!this->registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    // Read stream as text
    Core::String source = Core::StringStream(stream).ReadUntil(0);

    // Describe job
    NyLang::CompilerJobInfo job;
    job.source = source.AsCharArray();
    job.debugOutput = false;

    // Get common
    auto compiler = registry->GetInterface<NyLang::ICompiler>();
    auto diagnostic = registry->GetInterface<NyLang::IDiagnostic>();

    // Configure context
    auto shaderContext = registry->GetClass<NyShader::ShaderContext>();
    shaderContext->Reset();

    // Compile
    auto context = compiler->Compile(job);

    // Pretty print messages
    NyLang::PrettyStream prettyStream;
    diagnostic->PrettyPrint(prettyStream);
    diagnostic->Clear();

    // OK?
    if (!context->success) {
        ErrorLog(this).Write("Failed:\n", prettyStream.GetData().c_str(), "\n");
        return COM::kError;
    }

    // Dump messages
    Core::String info = prettyStream.GetData().c_str();
    if (info.Length() > 0) {
        InfoLog(this).Write("Ny compilation job finished (OK):\n", info);
    } else {
        InfoLog(this).Write("Ny compilation job finished (OK)");
    }

    // Optimize
    NyLang::IL::PassManager passes;
    {
        auto pass = registry->context.New<NyLang::IL::DeadIRPass>();
        pass->Initialize(false);
        passes.AddPass(pass);
    }
    passes.Pass(context->semantic->ilProgram);

    // Automatic entry points?
    if (config->entryPoints.Size() == 0) {
        auto root = context->semantic->ilProgram->GetContainers().front();

        // Find functions with kernel annotations
        Core::HashSet<Core::String> stageSets;
        for (auto function : root->GetFunctions()) {
            for (auto &annot : function->GetAnnotations()) {
                if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
                    if (meta->GetName() == "Kernel") {
                        if (auto string = NyLang::OuterCast<NyLang::IL::ConstantStringValue>(meta->GetMemberWithName("stage"))) {
                            if (stageSets.Contains(string->GetString().c_str())) {
                                ErrorLog(this).Write("Failed to automatically determine kernel entry points, multiple kernels with stage '", string->GetString().c_str(), "'");
                                return COM::kError;
                            }
                            stageSets.Add(string->GetString().c_str());
                        }
                        config->entryPoints.Add(function->GetName().c_str());
                        break;
                    }
                }
            }
        }

        // No entry points?
        if (config->entryPoints.Size() == 0) {
            ErrorLog(this).Write("Failed to automatically determine kernel entry points, no valid kernels found");
            return COM::kError;
        }

        // Diagnostic
        Core::String debug;
        for (USize i = 0; i < config->entryPoints.Size(); i++) {
            if (i != 0) {
                debug += ", ";
            }
            debug += config->entryPoints[i];
        }
        InfoLog(this).Write("Automatically determined entry points to kernels [", debug, "]");
    }

    // Foreach entry point
    for (const Core::String &entryPoint : config->entryPoints) {
        auto blob = new GHA::ShaderBlob();

        // Ensure entry point exists
        auto range = context->semantic->ilProgram->GetContainers().front()->GetFunctionsWithName(entryPoint.AsCharArray());
        if (range.first == range.second) {
            ErrorLog(this).Write("Entry point not found '", entryPoint, "'");
            return COM::kError;
        } else if (std::distance(range.first, range.second) > 1) {
            ErrorLog(this).Write("Multiple possible entry points '", entryPoint, "'");
            return COM::kError;
        }

        // Get kernel annotation
        NyLang::IL::ConstantMetaValue* kernel = nullptr;
        for (auto &annot : (*range.first).second->GetAnnotations()) {
            if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
                if (meta->GetName() == "Kernel") {
                    kernel = meta;
                }
            }
        }

        // Must have kernel annotation
        if (!kernel) {
            ErrorLog(this).Write("Kernel annotation not present in entry point '", entryPoint, "'");
            return COM::kError;
        }

        // Get stage name
        if (auto value = kernel->GetMemberWithName("stage")) {
            if (auto string = NyLang::OuterCast<NyLang::IL::ConstantStringValue>(value)) {
                if (string->GetString() == "vertex") {
                    blob->stage = GHA::Stage::eVertex;
                } else if (string->GetString() == "pixel") {
                    blob->stage = GHA::Stage::ePixel;
                } else if (string->GetString() == "domain") {
                    blob->stage = GHA::Stage::eDomain;
                } else if (string->GetString() == "hull") {
                    blob->stage = GHA::Stage::eHull;
                } else if (string->GetString() == "geometry") {
                    blob->stage = GHA::Stage::eGeometry;
                } else if (string->GetString() == "compute") {
                    blob->stage = GHA::Stage::eCompute;
                } else {
                    ErrorLog(this).Write("Invalid kernel stage '", string->GetString().c_str(), "'");
                    return COM::kError;
                }
            } else {
                ErrorLog(this).Write("Kernel stage is not of a string type\n");
                return COM::kError;
            }
        } else {
            ErrorLog(this).Write("Kernel annotation does not have a stage name\n");
            return COM::kError;
        }

        // Fill vertex input attributes
        if (blob->stage == GHA::Stage::eVertex) {
            for (auto &argument : (*range.first).second->GetArguments()) {
                if (!FillVertexAttributes(blob->inputAssembly, argument)) {
                    return COM::kError;
                }
            }
        }

        // Prepare nil desc
        ShaderCompileNILDesc nilDesc;
        nilDesc.desc = config;
        nilDesc.entryPoint = entryPoint;
        nilDesc.stage = blob->stage;

        // Run each nil compiler
        for (auto &&compiler : compilers) {
            auto &compiledBlob = blob->compiledBlobs[compiler.second->GetClassID()];
            if (!compiler.second->Compile(registry, nilDesc, context->semantic->ilProgram, blob, &compiledBlob)) {
                ErrorLog(this).Write("One of the registered nil compilers failed\n");
                return COM::kError;
            }
        }

        // ...
        asset->shaderBlobs[blob->stage] = blob;
    }

    // Diagnostic
    InfoLog(this).Write("Shader compilation finished (", timer.Delta(), "s)");

    // OK
    *out = asset;
    return COM::kOK;
}
