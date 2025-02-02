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

#include <GHA/Vulkan/GlassCompiler.h>
#include <NyLang/Backend/IL/Program.h>
#include <NyLang/Backend/IL/IType.h>
#include <NyLang/Backend/IL/Function.h>
#include <NyLang/Backend/IL/Argument.h>
#include <NyLang/Backend/IL/VoidType.h>
#include <NyLang/Native/IL/Types/ClassType.h>
#include <NyLang/Native/IL/Types/FPType.h>
#include <NyLang/Native/IL/Types/IntegerType.h>
#include <NyLang/Native/IL/Types/PointerType.h>
#include <NyLang/Native/IL/Types/ArrayType.h>
#include <NyLang/Native/IL/Types/VectorType.h>
#include <NyLang/Native/IL/Types/FunctionType.h>
#include <NyLang/Native/IL/Values/ConstantIntValue.h>
#include <NyLang/Native/IL/Values/ConstantArrayValue.h>
#include <NyLang/Native/IL/Values/ConstantMetaValue.h>
#include <NyLang/Native/IL/Values/ConstantDoubleValue.h>
#include <NyLang/Native/IL/Values/ConstantFloatValue.h>
#include <NyLang/Native/IL/Values/ConstantFunctionValue.h>
#include <NyLang/Native/IL/Values/ConstantUIntValue.h>
#include <NyLang/Native/IL/Instructions/AddInstruction.h>
#include <NyLang/Native/IL/Instructions/AllocaInstruction.h>
#include <NyLang/Native/IL/Instructions/BitAndInstruction.h>
#include <NyLang/Native/IL/Instructions/BitLShiftInstruction.h>
#include <NyLang/Native/IL/Instructions/CastF2IInstruction.h>
#include <NyLang/Native/IL/Instructions/CallInstruction.h>
#include <NyLang/Native/IL/Instructions/BitXorInstruction.h>
#include <NyLang/Native/IL/Instructions/BitRShiftInstruction.h>
#include <NyLang/Native/IL/Instructions/BitOrInstruction.h>
#include <NyLang/Native/IL/Instructions/CastI2FInstruction.h>
#include <NyLang/Native/IL/Instructions/BitNotInstruction.h>
#include <NyLang/Native/IL/Instructions/NegInstruction.h>
#include <NyLang/Native/IL/Instructions/CastI2IInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpLtInstruction.h>
#include <NyLang/Native/IL/Instructions/CastF2FInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpEqInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpGeInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpLeInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpNeqInstruction.h>
#include <NyLang/Native/IL/Instructions/DivInstruction.h>
#include <NyLang/Native/IL/Instructions/CmpGtInstruction.h>
#include <NyLang/Native/IL/Instructions/SubInstruction.h>
#include <NyLang/Native/IL/Instructions/ModInstruction.h>
#include <NyLang/Native/IL/Instructions/FunctionAddressInstruction.h>
#include <NyLang/Native/IL/Instructions/ReturnInstruction.h>
#include <NyLang/Native/IL/Instructions/StoreInstruction.h>
#include <NyLang/Native/IL/Instructions/LoadInstruction.h>
#include <NyLang/Native/IL/Instructions/FieldAddressInstruction.h>
#include <NyLang/Native/IL/Instructions/ElementAddressInstruction.h>
#include <NyLang/Native/IL/Types/ArrayType.h>
#include <NyLang/Native/IL/Instructions/ThisInstruction.h>
#include <NyLang/Native/IL/Instructions/MulInstruction.h>
#include <NyLang/Native/IL/Instructions/BrInstruction.h>
#include <NyLang/Native/IL/Instructions/BrCondInstruction.h>
#include <NyLang/Native/IL/Instructions/ElementInstruction.h>
#include <NyLang/Native/IL/Instructions/FieldInstruction.h>
#include <NyLang/Backend/IL/Container.h>
#include <NyLang/Backend/IL/Block.h>
#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Native/IL/Class.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/GlassManager.h>
#include <Core/Array.h>
#include <Core/HashMap.h>
#include <Data/NyShader/NILTypes.h>
#include <Data/NyShader/NILInstructions.h>
#include <lunarglass/Core/LunarGLASSTopIR.h>
#include <lunarglass/Core/TopBuilder.h>
#include <NyLang/Native/IL/NativeEmitter.h>
#include <NyLang/Native/IL/Values/ConstantStringValue.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <Core/HashSet.h>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

DJINN_NS3(GHA, Vulkan);

GlassCompiler::GlassCompiler(Host::IRegistry *registry, GlassManager *manager) : manager(manager), registry(registry) {
}

COM::Result GlassCompiler::FlattenInput(llvm::BasicBlock *block, NyLang::IL::IType *ilType, NyLang::IL::IValue *ilValue, llvm::Value *ptr, AnnotationChain chain, const Core::String &decoration) {
    llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
    gla::Builder glassBuilder(builder, manager, **metaData);
    builder.SetInsertPoint(block);

    // Convert IL type
    llvm::Type *type;
    if (!GetType(ilType, &type)) {
        return COM::kError;
    }

    // Annotation chain
    for (auto &&annot : ilValue->GetAnnotations()) {
        if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
            chain.Add(meta);
        }
    }

    // Class?
    if (auto _class = NyLang::OuterCast<NyLang::IL::ClassType>(ilType)) {
        // Flatten all members.
        UInt32 index = 0;
        for (auto &member : _class->GetContained()->GetMembers()) {
            if (!FlattenInput(
                    block,
                    member->GetType(),
                    member,
                    builder.CreateStructGEP(ptr, index),
                    chain,
                    Unique((decoration + "_" + member->GetName().c_str()).AsCharArray())
            )) {
                return COM::kError;
            }
            index++;
        }
    } else {
        // Get builtin
        gla::EMdBuiltIn builtin;
        if (!GetBuiltin(chain, true, builtin)) {
            return COM::kError;
        }

        if (builtin == gla::EMdBuiltIn::EmbCount) {
            // Store to target
            // builder.CreateStore(builder.CreateLoad(var), ptr);
            inputRegister++;
        } else {
            // Declare input
            auto var = glassBuilder.createVariable(gla::Builder::EStorageQualifier::ESQInput, 0, type, nullptr, nullptr, ("in_" + decoration).AsCharArray());

            // Determine interpolation mode
            int mode = 0;
            if (desc.stage == GHA::Stage::ePixel &&
                !(
                        var->getType()->getPointerElementType()->isIntegerTy() ||
                        (var->getType()->getPointerElementType()->isVectorTy() && var->getType()->getPointerElementType()->getVectorElementType()->isIntegerTy())
                )) {
                mode = gla::MakeInterpolationMode(gla::EInterpolationMethod::EIMSmooth, gla::EInterpolationLocation::EILFragment);
            }

            auto proxyValue = GetTypeProxy(type, (Core::String(var->getName().data()) + "_proxy").AsCharArray());

            // Prepare metadata
            metaData->makeMdInputOutput(
                    var->getName(),
                    gla::InputListMdName,
                    gla::EMdInputOutput::EMioPipeIn,
                    proxyValue,
                    gla::EMdTypeLayout::EMtlNone,
                    gla::EMdPrecision::EMpNone,
                    inputRegister++,
                    nullptr, nullptr,
                    mode,
                    builtin,
                    -1, 0, -1
            );

            // Store to target
            builder.CreateStore(builder.CreateLoad(var), ptr);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result GlassCompiler::FlattenOutput(llvm::BasicBlock *block, NyLang::IL::IType *ilType, NyLang::IL::IValue *ilValue, llvm::Value *value, AnnotationChain chain, const Core::String &decoration, int &offset) {
    llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
    gla::Builder glassBuilder(builder, manager, **metaData);
    builder.SetInsertPoint(block);

    // Convert IL type
    llvm::Type *type;
    if (!GetType(ilType, &type)) {
        return COM::kError;
    }

    // Annotation chain
    for (auto &&annot : ilValue->GetAnnotations()) {
        if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
            chain.Add(meta);
        }
    }

    // Class?
    if (auto _class = NyLang::OuterCast<NyLang::IL::ClassType>(ilType)) {
        // Flatten all members
        UInt32 index = 0;
        for (auto &member : _class->GetContained()->GetMembers()) {
            if (!FlattenOutput(
                    block,
                    member->GetType(),
                    member,
                    builder.CreateExtractValue(value, index),
                    chain,
                    Unique((decoration + "_" + member->GetName().c_str()).AsCharArray()),
                    ++offset
            )) {
                return COM::kError;
            }
            index++;
        }
    } else {
        auto &val = returnValueMap[offset];
        if (!val) {
            // Get builtin
            gla::EMdBuiltIn builtin;
            if (!GetBuiltin(chain, false, builtin)) {
                return COM::kError;
            }

            // Declare output
            auto var = glassBuilder.createVariable(gla::Builder::EStorageQualifier::ESQOutput, 0, type, nullptr, nullptr, ("out_" + decoration).AsCharArray());

            // Determine interpolation mode
            int mode = 0;
            if (desc.stage == GHA::Stage::eVertex) {
                mode = gla::MakeInterpolationMode(gla::EInterpolationMethod::EIMSmooth, gla::EInterpolationLocation::EILFragment);
            }

            // Prepare metadata
            metaData->makeMdInputOutput(
                    var->getName(),
                    gla::OutputListMdName,
                    gla::EMdInputOutput::EMioPipeOut,
                    GetTypeProxy(type, (Core::String(var->getName().data()) + "_proxy").AsCharArray()),
                    gla::EMdTypeLayout::EMtlNone,
                    gla::EMdPrecision::EMpNone,
                    outputRegister++,
                    nullptr, nullptr,
                    mode,
                    builtin,
                    -1, 0, -1
            );

            val = var;
        }


        // Store to target
        builder.CreateStore(value, val);
    }

    // OK
    return COM::kOK;
}

COM::Result GlassCompiler::Compile(NyLang::Registry* nyRegistry, const Data::ShaderCompileNILDesc &desc, NyLang::IL::Program *program, GHA::ShaderBlob* shaderBlob, Vulkan::CompiledShaderBlob *blob) {
    this->desc = desc;
    this->shaderBlob = shaderBlob;
    metaData = Core::MakeShared<gla::Metadata>(manager->getContext(), manager->getModule());
    layout = new llvm::DataLayout(manager->getModule());

    // Generate type map
    for (auto &container : program->GetContainers()) {
        if (auto _class = NyLang::OuterCast<NyLang::IL::Class>(container)) {
            auto type = llvm::StructType::create(manager->getContext());
            type->setName(Unique(_class->GetName().c_str()).AsCharArray());
            typeMap[_class] = type;
        }
    }

    // Fill types
    for (auto it : typeMap) {
        Core::Array<llvm::Type *> types;
        for (auto &member : it.first->GetMembers()) {
            // Convert IL type
            llvm::Type *type;
            if (!GetType(member->GetType(), &type)) {
                return COM::kError;
            }

            // ...
            types.Add(type);
        }
        it.second->setBody(types.handle);
    }

    // Find entry point
    NyLang::IL::Function *mainFunction = nullptr;
    for (auto &ilFunction : program->GetContainers()[0]->GetFunctions()) {
        if (desc.entryPoint == ilFunction->GetName().c_str()) {
            mainFunction = ilFunction;
        }
    }

    CompileFunctionList(mainFunction);

    // Fill externals
    for (auto &decl : program->GetContainers()[0]->GetMembers()) {
        if (decl->GetFlags() & NyLang::IL::MemberStorageFlag::eExtern) {
            bool ignore = false;
            for (auto &annot : decl->GetAnnotations()) {
                if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
                    if (meta->GetName() == "Packed") {
                        ignore = true;

                        auto bucket = meta->GetMemberWithName("bucket");
                        if (!bucket) {
                            ErrorLog(registry).Write("Packed annotation does not have a bucket member");
                            return COM::kError;
                        }

                        auto intVal = NyLang::OuterCast<NyLang::IL::ConstantIntValue>(bucket);
                        if (!intVal) {
                            ErrorLog(registry).Write("Packed bucket not an integer");
                            return COM::kError;
                        }

                        auto set = static_cast<UInt32>(intVal->GetValue());
                        auto index = registerOffsets[set]++;

                        if (IsVisible(decl)) {
                            // Add resource
                            PipelineScheduler::CompiledDescriptor descriptor;
                            descriptor.set = set;
                            descriptor.index = index;
                            descriptor.rid = PipelineScheduler::DescriptorType::eConstantBuffer;
                            descriptor.name = decl->GetName().c_str();
                            blob->resources[set].Add(descriptor);

                            llvm::Type *type;
                            if (!GetType(decl->GetType(), &type)) {
                                return COM::kError;
                            }

                            llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
                            gla::Builder glassBuilder(builder, manager, **metaData);

                            auto name = Unique(decl->GetName().c_str());
                            llvm::Value *proxy = GetTypeProxy(type, name + "_type");

                            valueMap[decl] = glassBuilder.createVariable(gla::Builder::EStorageQualifier::ESQUniform, 0, type, nullptr, nullptr, name.AsCharArray());

                            llvm::MDNode *layout = metaData->makeMdTypeLayout(
                                    gla::EMdTypeLayout::EMtlNone,
                                    gla::EMdPrecision::EMpNone,
                                    gla::MaxUserLayoutLocation,
                                    nullptr,
                                    gla::EInterpolationMethod::EIMNone,
                                    gla::EMdBuiltIn::EmbNone, GetLocation(index, set)
                            );

                            Core::Array<llvm::Value *> args;
                            if (type->isStructTy()) {
                                llvm::MDNode *aggregate;
                                if (!GetAggregateMD(
                                        NyLang::OuterCast<NyLang::IL::ClassType>(decl->GetType())->GetContained(),
                                        metaData->makeMdTypeLayout(
                                                gla::EMdTypeLayout::EMtlNone,
                                                gla::EMdPrecision::EMpNone,
                                                gla::MaxUserLayoutLocation,
                                                nullptr
                                        ),
                                        &aggregate,
                                        name
                                )) {
                                    return COM::kError;
                                }

                                args.Add(
                                        llvm::MDString::get(manager->getContext(), name.AsCharArray()),
                                        llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), gla::EMdInputOutput::EMioUniformBlockMember),
                                        proxy,
                                        layout,
                                        aggregate
                                );
                            } else {
                                args.Add(
                                        llvm::MDString::get(manager->getContext(), name.AsCharArray()),
                                        llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), gla::EMdInputOutput::EMioDefaultUniform),
                                        proxy,
                                        layout
                                );
                            }

                            manager->getModule()->getOrInsertNamedMetadata(gla::UniformListMdName)->addOperand(llvm::MDNode::get(manager->getContext(), args.handle));
                        }
                        break;
                    } else if (meta->GetName() == "Dynamic") {
                        ignore = true;

                        llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
                        gla::Builder glassBuilder(builder, manager, **metaData);

                        if (!dynamicStorage) {
                            dynamicStorageType = llvm::StructType::create(manager->getContext());
                            dynamicStorageType->setName("DynamicStorage");

                            dynamicStorage = glassBuilder.createVariable(gla::Builder::EStorageQualifier::ESQUniform, 0, dynamicStorageType, nullptr, nullptr, "dynamicStorage");

                            blob->dynamicOffset = blob->dynamicSize;
                        }

                        if (IsVisible(decl)) {
                            blob->dynamicBindingOffsets[Core::ComputeCRC64(decl->GetName().data())] = blob->dynamicOffset + blob->dynamicSize;
                        }

                        llvm::Type *type;
                        if (!GetType(decl->GetType(), &type)) {
                            return COM::kError;
                        }

                        blob->dynamicSize += static_cast<UInt32>(layout->getTypeAllocSize(type));

                        dynamicTypes.Add(type);
                        dynamicValues.Add(decl);

                        PackedProxy proxy;
                        proxy.source = dynamicStorage;
                        proxy.index = static_cast<UInt32>(dynamicTypes.Size() - 1);
                        packedValueMap.Add(decl, proxy);
                        break;
                    }
                }
            }

            if (!ignore) {
                llvm::Value *_dummy;
                if (!FlattenExtern(blob, decl->GetType(), decl, AnnotationChain(), IsVisible(decl), &_dummy, decl->GetName().c_str(), decl->GetName().c_str())) {
                    return COM::kError;
                }
            }
        }
    }

    // Fill dynamic storage body
    if (dynamicStorage) {
        // Set body
        dynamicStorageType->setBody(dynamicTypes.handle);
    }

    // Fill functions
    for (auto &ilContainer : program->GetContainers()) {
        // Foreach function
        for (auto &ilFunction : ilContainer->GetFunctions()) {
            if (!functionList.Contains(ilFunction)) {
                continue;
            }

            if (ilContainer != program->GetContainers().front()) {
                isFramed.Add(ilFunction);
            }

            auto functionType = NyLang::OuterCast<NyLang::IL::FunctionType>(ilFunction->GetType());

            // Outer block?
            llvm::Type *outerType = nullptr;
            if (auto _class = NyLang::OuterCast<NyLang::IL::Class>(ilContainer)) {
                // Must exist inside type map
                auto hashIt = typeMap.Find(_class);
                if (hashIt == typeMap.end()) {
                    ErrorLog(registry).Write("Unmapped type '", _class->GetName().c_str(), "'");
                    return COM::kError;
                }

                // ...
                outerType = (*hashIt).second;
            }

            // Entry point?
            if (desc.entryPoint == ilFunction->GetName().c_str()) {
                mainFunction = ilFunction;

                auto function = llvm::cast<llvm::Function>(manager->getModule()->getOrInsertFunction(
                        "main",
                        llvm::FunctionType::get(llvm::Type::getVoidTy(manager->getContext()), {}, false)
                ));

                function->addAttribute(llvm::AttributeSet::FunctionIndex, llvm::Attribute::AlwaysInline);

                // Prepare emitters
                llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());

                // Flatten all arguments
                for (auto &argument : ilFunction->GetArguments()) {
                    // Get type
                    llvm::Type *type;
                    if (!GetType(argument->GetType(), &type)) {
                        return COM::kError;
                    }

                    auto block = llvm::BasicBlock::Create(manager->getContext(), "flatten", function);
                    builder.SetInsertPoint(block);

                    // Dummy variable, should be optimized out later
                    auto alloc = builder.CreateAlloca(type);

                    // Flatten
                    Check(FlattenInput(block, argument->GetType(), argument, alloc, {}));

                    valueMap[argument] = builder.CreateLoad(alloc);
                }

                // Create function
                functionMap[ilFunction] = function;
            } else {
                // Collect argument types
                Core::Array<llvm::Type *> argumentTypes;
                if (outerType) {
                    argumentTypes.Add(llvm::PointerType::get(outerType, 0));
                }
                for (auto &arg : functionType->GetArgumentTypes()) {
                    llvm::Type *type;
                    if (!GetType(arg, &type)) {
                        return COM::kError;
                    }
                    argumentTypes.Add(type);
                }

                // Get return type
                llvm::Type *returnType;
                if (!GetType(functionType->GetReturnType(), &returnType)) {
                    return COM::kError;
                }

                // Determine name
                Core::String name = "";
                if (ilFunction->GetName() == "main") {
                    name = Unique("_main");
                } else {
                    name = Unique(ilFunction->GetName().c_str());
                }

                auto function = llvm::cast<llvm::Function>(manager->getModule()->getOrInsertFunction(
                        name.AsCharArray(),
                        llvm::FunctionType::get(returnType, argumentTypes.handle, false)
                ));

                function->addAttribute(llvm::AttributeSet::FunctionIndex, llvm::Attribute::AlwaysInline);

                // Create function
                functionMap[ilFunction] = function;
            }
        }
    }

    // Foreach container
    for (auto &container : program->GetContainers()) {
        // TODO: Initialization block

        // Foreach function
        for (auto &function : container->GetFunctions()) {
            if (!functionList.Contains(function)) {
                continue;
            }

            if (!CompileFunction(container, function, function->GetName().c_str() == desc.entryPoint)) {
                return COM::kError;
            }
        }
    }

    // Compile dynamic storage
    if (dynamicStorage) {
        // Dummy class
        auto _class = new NyLang::IL::Class(nyRegistry->context, "DynamicStorage");
        for (auto member : dynamicValues) {
            _class->AddMember(member->GetName(), member->GetType(), {});
        }

        // Dummy type
        typeMap.Add(_class, dynamicStorageType);

        auto name = "dynamicStorage";
        llvm::Value *proxy = GetTypeProxy(dynamicStorageType, "dynamicStorage_type");

        llvm::MDNode *layout = metaData->makeMdTypeLayout(
                gla::EMdTypeLayout::EMtlPushConstant,
                gla::EMdPrecision::EMpNone,
                gla::MaxUserLayoutLocation,
                nullptr,
                gla::EInterpolationMethod::EIMNone,
                gla::EMdBuiltIn::EmbNone, 0
        );

        llvm::MDNode *aggregate;
        if (!GetAggregateMD(
                _class,
                metaData->makeMdTypeLayout(
                        gla::EMdTypeLayout::EMtlNone,
                        gla::EMdPrecision::EMpNone,
                        gla::MaxUserLayoutLocation,
                        nullptr
                ),
                &aggregate,
                name
        )) {
            return COM::kError;
        }

        Core::Array<llvm::Value *> args;
        args.Add(
                llvm::MDString::get(manager->getContext(), name),
                llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), gla::EMdInputOutput::EMioUniformBlockMember),
                proxy,
                layout,
                aggregate
        );

        manager->getModule()->getOrInsertNamedMetadata(gla::UniformListMdName)->addOperand(llvm::MDNode::get(manager->getContext(), args.handle));
    }

    // OK
    return COM::kOK;
}

void GlassCompiler::CompileFunctionList(NyLang::IL::Function *function) {
    functionList.Add(function);
    for (auto &blocks : function->GetExecutionBlock()->GetBlocks()) {
        for (auto &instr : blocks->GetInstructions()) {
            switch (instr->GetClassID()) {
                case NyLang::IL::FunctionAddressInstruction::kCID: {
                    auto ptr = static_cast<NyLang::IL::FunctionAddressInstruction *>(instr);
                    if (!functionList.Contains(ptr->GetFunction())) {
                        CompileFunctionList(ptr->GetFunction());
                    }
                    break;
                }
                case NyLang::IL::CallInstruction::kCID: {
                    auto ptr = static_cast<NyLang::IL::CallInstruction *>(instr);
                    if (auto constant = NyLang::OuterCast<NyLang::IL::ConstantFunctionValue>(ptr->GetFunction())) {
                        if (!functionList.Contains(constant->GetFunction())) {
                            CompileFunctionList(constant->GetFunction());
                        }
                    }
                    break;
                }
            }
        }
    }
}

bool GlassCompiler::IsVisible(NyLang::IL::IValue *value) {
    for (auto function : functionList) {
        for (auto &blocks : function->GetExecutionBlock()->GetBlocks()) {
            for (auto &instr : blocks->GetInstructions()) {
                for (auto &op : instr->GetOperands()) {
                    if (op == value) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

COM::Result GlassCompiler::CompileFunction(NyLang::IL::Container *ilContainer, NyLang::IL::Function *ilFunction, bool isMain) {
    llvm::Function *function = functionMap[ilFunction];

    // Prepare emitters
    llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
    gla::Builder glassBuilder(builder, manager, **metaData);

    // Fill argument value map
    auto paramIt = ilFunction->GetArguments().begin();
    auto begin = function->getArgumentList().begin();
    if (ilContainer->GetClassID() == NyLang::IL::Class::kCID) {
        begin++;
    }
    for (; begin != function->getArgumentList().end(); begin++, paramIt++) {
        valueMap[(*paramIt)] = &*begin;
    }

    // Create blocks
    for (auto &ilBlock : ilFunction->GetExecutionBlock()->GetBlocks()) {
        blockMap[ilBlock] = llvm::BasicBlock::Create(manager->getContext(), "B", function);
    }

    if (isMain && (function->getBasicBlockList().size() > 1)) {
        builder.SetInsertPoint(&function->getBasicBlockList().front());
        builder.CreateBr(++function->getBasicBlockList().begin());
    }

    // Foreach block in execution block
    for (auto &ilBlock : ilFunction->GetExecutionBlock()->GetBlocks()) {
        builder.SetInsertPoint(blockMap[ilBlock]);

        // Foreach instruction
        for (auto &ilInstruction : ilBlock->GetInstructions()) {
            switch (ilInstruction->GetClassID()) {
                case NyLang::IL::AddInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::AddInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFAdd(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateAdd(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::AllocaInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::AllocaInstruction *>(ilInstruction);
                    llvm::Type *type;
                    if (!GetType(instr->GetAllocaType(), &type)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateAlloca(type);
                    break;
                }
                case NyLang::IL::BitAndInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitAndInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateAnd(lhs, rhs);
                    break;
                }
                case NyLang::IL::BitLShiftInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitLShiftInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateShl(lhs, rhs);
                    break;
                }
                case NyLang::IL::BitNotInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitNotInstruction *>(ilInstruction);
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateNot(value);
                    break;
                }
                case NyLang::IL::NegInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::NegInstruction *>(ilInstruction);
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    if (instr->GetValue()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFNeg(value);
                    } else {
                        valueMap[instr] = builder.CreateNeg(value);
                    }
                    break;
                }
                case NyLang::IL::BitOrInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitOrInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateOr(lhs, rhs);
                    break;
                }
                case NyLang::IL::BitRShiftInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitRShiftInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateLShr(lhs, rhs);
                    break;
                }
                case NyLang::IL::BitXorInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitXorInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateXor(lhs, rhs);
                    break;
                }
                case NyLang::IL::BrCondInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BrCondInstruction *>(ilInstruction);
                    llvm::Value *cond;
                    if (!GetValue(instr->GetCondition(), builder.GetInsertBlock(), &cond)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateCondBr(cond, blockMap[instr->GetTrueBlock()], blockMap[instr->GetFalseBlock()]);
                    break;
                }
                case NyLang::IL::BrInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BrInstruction *>(ilInstruction);
                    if (!builder.GetInsertBlock()->getTerminator()) {
                        valueMap[instr] = builder.CreateBr(blockMap[instr->GetBlock()]);
                    }
                    break;
                }
                case NyLang::IL::CallInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CallInstruction *>(ilInstruction);

                    Core::Array<llvm::Value *> arguments;

                    llvm::Value *func;
                    {
                        auto frame = functionFrameMap.Find(instr->GetFunction());
                        if (frame != functionFrameMap.End()) {
                            if ((*frame).second.first) {
                                arguments.Add((*frame).second.first);
                            }
                            func = (*frame).second.second;
                        } else {
                            if (!GetValue(instr->GetFunction(), builder.GetInsertBlock(), &func)) {
                                return COM::kError;
                            }
                        }
                    }

                    for (auto it = instr->GetArgIt(); it < instr->GetOperands().end(); ++it) {
                        llvm::Value *value;
                        if (!GetValue((*it), builder.GetInsertBlock(), &value)) {
                            return COM::kError;
                        }
                        arguments.Add(value);
                    }
                    valueMap[instr] = builder.CreateCall(func, arguments.handle);
                    break;
                }
                case NyLang::IL::CastF2FInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CastF2FInstruction *>(ilInstruction);
                    llvm::Type *type;
                    if (!GetType(instr->GetType(), &type)) {
                        return COM::kError;
                    }
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateFPCast(value, type);
                    break;
                }
                case NyLang::IL::CastF2IInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CastF2IInstruction *>(ilInstruction);
                    llvm::Type *type;
                    if (!GetType(instr->GetType(), &type)) {
                        return COM::kError;
                    }
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    if (instr->IsUnsigned()) {
                        valueMap[instr] = builder.CreateFPToUI(value, type);
                    } else {
                        valueMap[instr] = builder.CreateFPToSI(value, type);
                    }
                    break;
                }
                case NyLang::IL::CastI2FInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CastI2FInstruction *>(ilInstruction);
                    llvm::Type *type;
                    if (!GetType(instr->GetType(), &type)) {
                        return COM::kError;
                    }
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetValue()->GetType())->IsUnsigned()) {
                        valueMap[instr] = builder.CreateUIToFP(value, type);
                    } else {
                        valueMap[instr] = builder.CreateSIToFP(value, type);
                    }
                    break;
                }
                case NyLang::IL::CastI2IInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CastI2IInstruction *>(ilInstruction);
                    llvm::Type *type;
                    if (!GetType(instr->GetType(), &type)) {
                        return COM::kError;
                    }
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateIntCast(value, type, instr->IsUnsigned());
                    break;
                }
                case NyLang::IL::CmpEqInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CmpEqInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpOEQ(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateICmpEQ(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::CmpGeInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CmpGeInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpOGE(lhs, rhs);
                    } else {
                        if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetLHS()->GetType())->IsUnsigned()) {
                            valueMap[instr] = builder.CreateICmpUGE(lhs, rhs);
                        } else {
                            valueMap[instr] = builder.CreateICmpSGE(lhs, rhs);
                        }
                    }
                    break;
                }
                case NyLang::IL::CmpGtInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitXorInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpOGT(lhs, rhs);
                    } else {
                        if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetLHS()->GetType())->IsUnsigned()) {
                            valueMap[instr] = builder.CreateICmpUGT(lhs, rhs);
                        } else {
                            valueMap[instr] = builder.CreateICmpSGT(lhs, rhs);
                        }
                    }
                    break;
                }
                case NyLang::IL::CmpLeInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CmpLeInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpOLE(lhs, rhs);
                    } else {
                        if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetLHS()->GetType())->IsUnsigned()) {
                            valueMap[instr] = builder.CreateICmpULE(lhs, rhs);
                        } else {
                            valueMap[instr] = builder.CreateICmpSLE(lhs, rhs);
                        }
                    }
                    break;
                }
                case NyLang::IL::CmpLtInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CmpLtInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpOLT(lhs, rhs);
                    } else {
                        if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetLHS()->GetType())->IsUnsigned()) {
                            valueMap[instr] = builder.CreateICmpULT(lhs, rhs);
                        } else {
                            valueMap[instr] = builder.CreateICmpSLT(lhs, rhs);
                        }
                    }
                    break;
                }
                case NyLang::IL::CmpNeqInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::CmpNeqInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFCmpONE(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateICmpNE(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::DivInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitXorInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFDiv(lhs, rhs);
                    } else if (NyLang::OuterCast<NyLang::IL::IntegerType>(instr->GetLHS()->GetType())->IsUnsigned()) {
                        valueMap[instr] = builder.CreateUDiv(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateSDiv(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::ElementAddressInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::ElementAddressInstruction *>(ilInstruction);
                    llvm::Value *index, *array;
                    if (!GetValue(instr->GetIndex(), builder.GetInsertBlock(), &index) || !GetValue(instr->GetArray(), builder.GetInsertBlock(), &array)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateGEP(array, std::vector<llvm::Value *>{llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), 0), index});
                    break;
                }
                case NyLang::IL::ElementInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::ElementInstruction *>(ilInstruction);
                    llvm::Value *index, *array;
                    if (!GetValue(instr->GetIndex(), builder.GetInsertBlock(), &index) || !GetValue(instr->GetArray(), builder.GetInsertBlock(), &array)) {
                        return COM::kError;
                    }
                    //array->dump();
                    //std::cerr << "\n";
                    //index->dump();
                    valueMap[instr] = builder.CreateLoad(builder.CreateGEP(array, std::vector<llvm::Value *>{llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), 0), index}));
                    //valueMap[instr] = builder.CreateLoad(builder.CreateValue(array, std::vector<llvm::Value *>{llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), 0), index}));
                    break;
                }
                case NyLang::IL::FieldAddressInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::FieldAddressInstruction *>(ilInstruction);
                    if (!instr->GetAddress()) {
                        llvm::Value *value;
                        if (!GetValue(instr->GetMember(), builder.GetInsertBlock(), &value)) {
                            return COM::kError;
                        }

                        valueMap[instr] = value;
                    } else {
                        llvm::Value *frame;
                        if (!GetValue(instr->GetAddress(), builder.GetInsertBlock(), &frame)) {
                            return COM::kError;
                        }
                        switch (instr->GetMember()->GetClassID()) {
                            case NyLang::IL::Member::kCID: {
                                valueMap[instr] = builder.CreateStructGEP(frame, static_cast<UInt32>(static_cast<NyLang::IL::Member *>(instr->GetMember())->GetIndex()));
                                break;
                            }
                            default: {
                                ErrorLog(registry).Write("NIL contains a non constant field address instruction");
                                return COM::kError;
                            }
                        }
                    }
                    break;
                }
                case NyLang::IL::FieldInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::FieldInstruction *>(ilInstruction);
                    if (!instr->GetValue() || valueMap.Contains(instr->GetMember()) || packedValueMap.Contains(instr->GetMember())) {
                        llvm::Value *value;
                        if (!GetValue(instr->GetMember(), builder.GetInsertBlock(), &value)) {
                            return COM::kError;
                        }

                        valueMap[instr] = builder.CreateLoad(value);
                    } else {
                        llvm::Value *valueFrame;
                        if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &valueFrame)) {
                            return COM::kError;
                        }
                        switch (instr->GetMember()->GetClassID()) {
                            case NyLang::IL::Member::kCID: {
                                valueMap[instr] = builder.CreateExtractValue(valueFrame, static_cast<UInt32>(static_cast<NyLang::IL::Member *>(instr->GetMember())->GetIndex()));
                                break;
                            }
                            default: {
                                ErrorLog(registry).Write("NIL contains a non constant field instruction");
                                return COM::kError;
                            }
                        }
                    }
                    break;
                }
                case NyLang::IL::FunctionAddressInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::FunctionAddressInstruction *>(ilInstruction);
                    llvm::Value *frame;
                    if (!GetValue(instr->GetAddress(), builder.GetInsertBlock(), &frame)) {
                        return COM::kError;
                    }

                    if (isFramed.Contains(instr->GetFunction())) {
                        //valueMap[instr] = functionMap[instr->GetFunction()];
                        functionFrameMap[instr] = {frame, llvm::cast<llvm::Function>(functionMap[instr->GetFunction()])};
                    } else {
                        functionFrameMap[instr] = {nullptr, llvm::cast<llvm::Function>(functionMap[instr->GetFunction()])};
                    }
                    break;
                }
                case NyLang::IL::LoadInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::LoadInstruction *>(ilInstruction);
                    llvm::Value *address;
                    if (!GetValue(instr->GetAddress(), builder.GetInsertBlock(), &address)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateLoad(address);
                    break;
                }
                case NyLang::IL::StoreInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::StoreInstruction *>(ilInstruction);
                    llvm::Value *address, *value;
                    if (!GetValue(instr->GetAddress(), builder.GetInsertBlock(), &address) || !GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    valueMap[instr] = builder.CreateStore(value, address);
                    break;
                }
                case NyLang::IL::ModInstruction::kCID: {
                    /*auto instr = (NyLang::IL::BitXorInstruction*)ilInstruction;
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateM(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateMul(lhs, rhs);
                    }*/
                    break;
                }
                case NyLang::IL::MulInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitXorInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFMul(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateMul(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::ReturnInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::ReturnInstruction *>(ilInstruction);
                    llvm::Value *value;
                    if (!GetValue(instr->GetValue(), builder.GetInsertBlock(), &value)) {
                        return COM::kError;
                    }
                    if (!builder.GetInsertBlock()->getTerminator()) {
                        if (isMain) {
                            int offset = 0;
                            Check(FlattenOutput(builder.GetInsertBlock(), instr->GetValue()->GetType(), instr->GetValue(), value, {}, "var", offset));
                            builder.CreateRetVoid();
                        } else {
                            if (instr->GetValue()) {
                                valueMap[instr] = builder.CreateRet(value);
                            } else {
                                valueMap[instr] = builder.CreateRetVoid();
                            }
                        }
                    }
                    break;
                }
                case NyLang::IL::SubInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::BitXorInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (instr->GetLHS()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFSub(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateSub(lhs, rhs);
                    }
                    break;
                }
                case NyLang::IL::ThisInstruction::kCID: {
                    auto instr = static_cast<NyLang::IL::ThisInstruction *>(ilInstruction);
                    valueMap[instr] = &function->getArgumentList().front();
                    break;
                }
                case Data::NyShader::VectorSetInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorSetInstruction *>(ilInstruction);
                    instr->GetOperands();
                    Core::Array<llvm::Value *> values;
                    for (auto ilValue : instr->GetValues()) {
                        llvm::Value *value;
                        if (!GetValue(ilValue, builder.GetInsertBlock(), &value)) {
                            return COM::kError;
                        }

                        // ...
                        values.Add(value);
                    }

                    llvm::Type *type;
                    if (!GetType(instr->GetResultType(), &type)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createConstructor(gla::EMdPrecision::EMpNone, values.handle, llvm::UndefValue::get(type));
                    break;
                }
                case Data::NyShader::VectorSwizzleInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorSwizzleInstruction *>(ilInstruction);

                    llvm::Value *vec;
                    if (!GetValue(instr->GetVector(), builder.GetInsertBlock(), &vec)) {
                        return COM::kError;
                    }

                    llvm::Type *type;
                    if (!GetType(instr->GetResultType(), &type)) {
                        return COM::kError;
                    }

                    std::vector<int> coms;
                    for (auto com : instr->coms.handle) {
                        coms.push_back(com);
                    }

                    valueMap[instr] = glassBuilder.createSwizzle(
                            gla::EMdPrecision::EMpNone,
                            vec,
                            coms,
                            type
                    );
                    break;
                }
                case Data::NyShader::VectorAddInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorAddInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (!rhs->getType()->isVectorTy()) {
                        rhs = builder.CreateVectorSplat(lhs->getType()->getVectorNumElements(), rhs);
                    }
                    if (NyLang::OuterCast<NyLang::IL::VectorType>(instr->GetLHS()->GetType())->GetContained()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFAdd(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateAdd(lhs, rhs);
                    }
                    break;
                }
                case Data::NyShader::VectorSubInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorSubInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (!rhs->getType()->isVectorTy()) {
                        rhs = builder.CreateVectorSplat(lhs->getType()->getVectorNumElements(), rhs);
                    }
                    if (NyLang::OuterCast<NyLang::IL::VectorType>(instr->GetLHS()->GetType())->GetContained()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFSub(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateSub(lhs, rhs);
                    }
                    break;
                }
                case Data::NyShader::VectorMulInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorMulInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (!rhs->getType()->isVectorTy()) {
                        rhs = builder.CreateVectorSplat(lhs->getType()->getVectorNumElements(), rhs);
                    }
                    if (NyLang::OuterCast<NyLang::IL::VectorType>(instr->GetLHS()->GetType())->GetContained()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFMul(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateMul(lhs, rhs);
                    }
                    break;
                }
                case Data::NyShader::VectorDivInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorMulInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    if (!rhs->getType()->isVectorTy()) {
                        rhs = builder.CreateVectorSplat(lhs->getType()->getVectorNumElements(), rhs);
                    }
                    auto contained = NyLang::OuterCast<NyLang::IL::VectorType>(instr->GetLHS()->GetType())->GetContained();
                    if (contained->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = builder.CreateFDiv(lhs, rhs);
                    } else if (NyLang::OuterCast<NyLang::IL::IntegerType>(contained)->IsUnsigned()) {
                        valueMap[instr] = builder.CreateUDiv(lhs, rhs);
                    } else {
                        valueMap[instr] = builder.CreateSDiv(lhs, rhs);
                    }
                    break;
                }
                case Data::NyShader::MatrixSetInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::MatrixSetInstruction *>(ilInstruction);
                    instr->GetOperands();
                    Core::Array<llvm::Value *> values;
                    for (auto ilValue : instr->GetValues()) {
                        llvm::Value *value;
                        if (!GetValue(ilValue, builder.GetInsertBlock(), &value)) {
                            return COM::kError;
                        }

                        // ...
                        values.Add(value);
                    }

                    llvm::Type *type;
                    if (!GetType(instr->GetResultType(), &type)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createMatrixConstructor(gla::EMdPrecision::EMpNone, values.handle, llvm::UndefValue::get(type));
                    break;
                }
                case Data::NyShader::MatrixMulInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorMulInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = glassBuilder.createMatrixMultiply(gla::EMdPrecision::EMpNone, lhs, rhs);
                    break;
                }
                case Data::NyShader::MatrixTransformInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::MatrixTransformInstruction *>(ilInstruction);
                    llvm::Value *lhs, *rhs;
                    if (!GetValue(instr->GetLHS(), builder.GetInsertBlock(), &lhs) || !GetValue(instr->GetRHS(), builder.GetInsertBlock(), &rhs)) {
                        return COM::kError;
                    }
                    valueMap[instr] = glassBuilder.createMatrixMultiply(gla::EMdPrecision::EMpNone, rhs, lhs);
                    break;
                }
                case Data::NyShader::TextureSampleGradientInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::TextureSampleGradientInstruction *>(ilInstruction);
                    llvm::Value *texture, *texcoord;
                    if (!GetValue(instr->GetTexture(), builder.GetInsertBlock(), &texture) || !GetValue(instr->GetTexcoord(), builder.GetInsertBlock(), &texcoord)) {
                        return COM::kError;
                    }

                    auto textureResource = NyLang::OuterCast<Data::NyShader::TextureResourceType>(instr->GetTexture()->GetType());
                    if (!textureResource) {
                        NyLang::PrettyStream stream;
                        NyLang::IL::PrettyPrintContext context;
                        instr->GetTexture()->GetType()->PrettyPrint(stream, context);
                        ErrorLog(registry).Write("Invalid texture sample instruction target resource '", stream.GetData().c_str(), "'");
                        return COM::kError;
                    }

                    llvm::Type *result;
                    if (!GetType(instr->GetResultType(), &result)) {
                        return COM::kError;
                    }

                    // Determine sampler type
                    gla::ESamplerType samplerType = {};
                    switch (textureResource->dim) {
                        case 1:
                            samplerType = gla::ESamplerType::ESampler1D;
                            break;
                        case 2:
                            samplerType = gla::ESamplerType::ESampler2D;
                            break;
                        case 3:
                            samplerType = gla::ESamplerType::ESampler3D;
                            break;
                    }
                    switch (textureResource->type) {
                        default:
                            break;
                        case Data::NyShader::TextureType::eBuffer:
                            samplerType = gla::ESamplerType::ESamplerBuffer;
                            break;
                        case Data::NyShader::TextureType::eCube:
                            samplerType = gla::ESamplerType::ESamplerCube;
                            break;
                    }

                    gla::ETextureFlags textureFlags = {};
                    //textureFlags |= gla::ETextureFlags::

                    gla::Builder::TextureParameters params = {};
                    params.ETPCoords = texcoord;
                    params.ETPSampler = texture;

                    llvm::Value *val = glassBuilder.createTextureCall(
                            gla::EMdPrecision::EMpNone,
                            llvm::VectorType::get(result->isVectorTy() ? result->getVectorElementType() : result, 4),
                            samplerType, textureFlags, params
                    );

                    if (result->isVectorTy()) {
                        std::vector<int> channels;
                        for (UInt32 i = 0; i < result->getVectorNumElements(); i++) {
                            channels.push_back(i);
                        }

                        valueMap[instr] = glassBuilder.createSwizzle(gla::EMdPrecision::EMpNone, val, channels, result);
                    } else {
                        std::vector<int> channels = {0};
                        valueMap[instr] = builder.CreateExtractElement(val, llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), 0));
                    }
                    break;
                }
                case Data::NyShader::TextureSampleTexelInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::TextureSampleGradientInstruction *>(ilInstruction);
                    llvm::Value *texture, *texcoord;
                    if (!GetValue(instr->GetTexture(), builder.GetInsertBlock(), &texture) || !GetValue(instr->GetTexcoord(), builder.GetInsertBlock(), &texcoord)) {
                        return COM::kError;
                    }

                    auto textureResource = NyLang::OuterCast<Data::NyShader::TextureResourceType>(instr->GetTexture()->GetType());
                    if (!textureResource) {
                        NyLang::PrettyStream stream;
                        NyLang::IL::PrettyPrintContext context;
                        instr->GetTexture()->PrettyPrint(stream, context);
                        ErrorLog(registry).Write("Invalid texture sample instruction target resource '", stream.GetData().c_str(), "'");
                        return COM::kError;
                    }

                    llvm::Type *result;
                    if (!GetType(instr->GetResultType(), &result)) {
                        return COM::kError;
                    }

                    gla::ESamplerType samplerType = {};
                    switch (textureResource->dim) {
                        default:
                            break;
                        case 1:
                            samplerType = gla::ESamplerType::ESampler1D;
                            break;
                        case 2:
                            samplerType = gla::ESamplerType::ESampler2D;
                            break;
                        case 3:
                            samplerType = gla::ESamplerType::ESampler3D;
                            break;
                    }
                    switch (textureResource->type) {
                        default:
                            break;
                        case Data::NyShader::TextureType::eBuffer:
                            samplerType = gla::ESamplerType::ESamplerBuffer;
                            break;
                        case Data::NyShader::TextureType::eCube:
                            samplerType = gla::ESamplerType::ESamplerCube;
                            break;
                    }

                    gla::ETextureFlags textureFlags = {};
                    textureFlags = (gla::ETextureFlags::ETFFetch);

                    gla::Builder::TextureParameters params = {};
                    params.ETPCoords = texcoord;
                    params.ETPSampler = texture;

                    valueMap[instr] = glassBuilder.createTextureCall(gla::EMdPrecision::EMpNone, result, samplerType, textureFlags, params);
                    break;
                }
                case Data::NyShader::SmoothstepInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::SmoothstepInstruction *>(ilInstruction);
                    llvm::Value *a, *b, *t;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b) || !GetValue(instr->GetT(), builder.GetInsertBlock(), &t)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fSmoothStep, a, b, t);
                    break;
                }
                case Data::NyShader::LerpInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::LerpInstruction *>(ilInstruction);
                    llvm::Value *a, *b, *t;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b) || !GetValue(instr->GetT(), builder.GetInsertBlock(), &t)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fMix, a, b, t);
                    break;
                }
                case Data::NyShader::ClampInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::ClampInstruction *>(ilInstruction);
                    llvm::Value *v, *a, *b;
                    if (!GetValue(instr->GetV(), builder.GetInsertBlock(), &v) || !GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }

                    llvm::Type *type = v->getType()->isVectorTy() ? v->getType()->getVectorElementType() : v->getType();
                    NyLang::IL::IType *ilType = v->getType()->isVectorTy() ? NyLang::StaticCast<Data::NyShader::VectorType>(instr->GetV()->GetType())->type : instr->GetV()->GetType();

                    if (type->isFloatingPointTy()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fClamp, v, a, b);
                    } else if (type->isIntegerTy() && NyLang::StaticCast<NyLang::IL::IntegerType>(ilType)->IsUnsigned()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_uClamp, v, a, b);
                    } else {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_sClamp, v, a, b);
                    }
                    break;
                }
                case Data::NyShader::MinInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::MinInstruction *>(ilInstruction);
                    llvm::Value *a, *b;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }

                    llvm::Type *type = a->getType()->isVectorTy() ? a->getType()->getVectorElementType() : a->getType();
                    NyLang::IL::IType *ilType = a->getType()->isVectorTy() ? NyLang::StaticCast<Data::NyShader::VectorType>(instr->GetA()->GetType())->type : instr->GetA()->GetType();

                    if (type->isFloatingPointTy()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fMin, a, b);
                    } else if (type->isIntegerTy() && NyLang::StaticCast<NyLang::IL::IntegerType>(ilType)->IsUnsigned()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_uMin, a, b);
                    } else {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_sMin, a, b);
                    }
                    break;
                }
                case Data::NyShader::MaxInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::MaxInstruction *>(ilInstruction);
                    llvm::Value *a, *b;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }

                    llvm::Type *type = a->getType()->isVectorTy() ? a->getType()->getVectorElementType() : a->getType();
                    NyLang::IL::IType *ilType = a->getType()->isVectorTy() ? NyLang::StaticCast<Data::NyShader::VectorType>(instr->GetA()->GetType())->type : instr->GetA()->GetType();

                    if (type->isFloatingPointTy()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fMax, a, b);
                    } else if (type->isIntegerTy() && NyLang::StaticCast<NyLang::IL::IntegerType>(ilType)->IsUnsigned()) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_uMax, a, b);
                    } else {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_sMax, a, b);
                    }
                    break;
                }
                case Data::NyShader::AbsInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::AbsInstruction *>(ilInstruction);
                    llvm::Value *a;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a)) {
                        return COM::kError;
                    }

                    if (instr->GetA()->GetType()->GetClassID() == NyLang::IL::FPType::kCID) {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fAbs, a);
                    } else {
                        valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_abs, a);
                    }
                    break;
                }
                case Data::NyShader::SqrtInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::SqrtInstruction *>(ilInstruction);
                    llvm::Value *a;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fSqrt, a);
                    break;
                }
                case Data::NyShader::AsFloatInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::AsFloatInstruction *>(ilInstruction);
                    llvm::Value *a;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fIntBitsTofloat, a);
                    break;
                }
                case Data::NyShader::AsUIntInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::AsFloatInstruction *>(ilInstruction);
                    llvm::Value *a;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fFloatBitsToInt, a);
                    break;
                }
                case Data::NyShader::AsIntInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::AsFloatInstruction *>(ilInstruction);
                    llvm::Value *a;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fFloatBitsToInt, a);
                    break;
                }
                case Data::NyShader::DotInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::DotInstruction *>(ilInstruction);
                    llvm::Value *a, *b;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }

                    switch (a->getType()->getVectorNumElements()) {
                        case 2:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot2, a, b);
                            break;
                        case 3:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot3, a, b);
                            break;
                        case 4:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot4, a, b);
                            break;
                    }
                    break;
                }
                case Data::NyShader::DistanceInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::DistanceInstruction *>(ilInstruction);
                    llvm::Value *a, *b;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }
                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDistance, a, b);
                    break;
                }
                case Data::NyShader::CrossInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::CrossInstruction *>(ilInstruction);
                    llvm::Value *a, *b;
                    if (!GetValue(instr->GetA(), builder.GetInsertBlock(), &a) || !GetValue(instr->GetB(), builder.GetInsertBlock(), &b)) {
                        return COM::kError;
                    }

                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fCross, a, b);
                    break;
                }
                case Data::NyShader::VectorLengthInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorLengthInstruction *>(ilInstruction);
                    llvm::Value *v;
                    if (!GetValue(instr->GetV(), builder.GetInsertBlock(), &v)) {
                        return COM::kError;
                    }
                    valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fLength, v);
                    break;
                }
                case Data::NyShader::VectorLength2Instruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorLength2Instruction *>(ilInstruction);
                    llvm::Value *v;
                    if (!GetValue(instr->GetV(), builder.GetInsertBlock(), &v)) {
                        return COM::kError;
                    }
                    switch (v->getType()->getVectorNumElements()) {
                        case 2:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot2, v, v);
                            break;
                        case 3:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot3, v, v);
                            break;
                        case 4:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot4, v, v);
                            break;
                    }
                    break;
                }
                case Data::NyShader::VectorSumInstruction::kCID: {
                    auto instr = static_cast<Data::NyShader::VectorSumInstruction *>(ilInstruction);
                    llvm::Value *v;
                    if (!GetValue(instr->GetV(), builder.GetInsertBlock(), &v)) {
                        return COM::kError;
                    }
                    llvm::Value *one;
                    switch (v->getType()->getVectorElementType()->getTypeID()) {
                        case llvm::Type::TypeID::IntegerTyID:
                            one = llvm::ConstantInt::get(v->getType()->getVectorElementType(), 0);
                            break;
                        case llvm::Type::TypeID::FloatTyID:
                        case llvm::Type::TypeID::DoubleTyID:
                            one = llvm::ConstantFP::get(v->getType()->getVectorElementType(), 0);
                            break;
                        default:
                            ErrorLog(registry).Write("Unknown vector base type");
                            return COM::kError;
                    }
                    switch (v->getType()->getVectorNumElements()) {
                        case 2:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot2, v, builder.CreateVectorSplat(2, one));
                            break;
                        case 3:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot3, v, builder.CreateVectorSplat(3, one));
                            break;
                        case 4:
                            valueMap[instr] = glassBuilder.createIntrinsicCall(gla::EMdPrecision::EMpNone, llvm::Intrinsic::gla_fDot4, v, builder.CreateVectorSplat(4, one));
                            break;
                    }
                    break;
                }
            }
        }

        // Void terminator
        if (function->getReturnType()->isVoidTy() && !builder.GetInsertBlock()->getTerminator()) {
            builder.CreateRetVoid();
        }
    }

    // OK
    return COM::kOK;
}

COM::Result GlassCompiler::GetType(NyLang::IL::IType *type, llvm::Type **out) {
    llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
    gla::Builder glassBuilder(builder, manager, **metaData);
    switch (type->GetClassID()) {
        case NyLang::IL::VoidType::kCID:
            *out = llvm::Type::getVoidTy(manager->getContext());
            return COM::kOK;
        case NyLang::IL::FPType::kCID:
            if (type->GetBitWidth() == 64) {
                *out = llvm::Type::getDoubleTy(manager->getContext());
                return COM::kOK;
            } else {
                *out = llvm::Type::getFloatTy(manager->getContext());
                return COM::kOK;
            }
        case NyLang::IL::IntegerType::kCID:
            switch (type->GetBitWidth()) {
                case 1:
                    *out = llvm::Type::getInt1Ty(manager->getContext());
                    return COM::kOK;
                case 8:
                    *out = llvm::Type::getInt8Ty(manager->getContext());
                    return COM::kOK;
                case 16:
                    *out = llvm::Type::getInt16Ty(manager->getContext());
                    return COM::kOK;
                case 32:
                    *out = llvm::Type::getInt32Ty(manager->getContext());
                    return COM::kOK;
                case 64:
                    *out = llvm::Type::getInt64Ty(manager->getContext());
                    return COM::kOK;
            }
            Always("Bad bitwidth");
            return COM::kError;
        case NyLang::IL::ArrayType::kCID: {
            auto array = static_cast<NyLang::IL::ArrayType *>(type);
            llvm::Type *contained;
            if (!GetType(array->GetContained(), &contained)) {
                return COM::kError;
            }
            *out = llvm::ArrayType::get(contained, static_cast<USize>(array->GetCount()));
            return COM::kOK;
        }
        case NyLang::IL::PointerType::kCID: {
            auto pointer = static_cast<NyLang::IL::PointerType *>(type);
            llvm::Type *contained;
            if (!GetType(pointer->GetContained(), &contained)) {
                return COM::kError;
            }
            *out = glassBuilder.getPointerType(contained, gla::Builder::EStorageQualifier::ESQLocal, 0);
            return COM::kOK;
        }
        case NyLang::IL::FunctionType::kCID:
            ErrorLog(registry).Write("NIL contains a function type as member, this is disallowed");
            return COM::kError;
        case NyLang::IL::ClassType::kCID: {
            auto classType = static_cast<NyLang::IL::ClassType *>(type);

            // Must exist inside type map
            auto hashIt = typeMap.Find(classType->GetContained());
            if (hashIt == typeMap.end()) {
                ErrorLog(registry).Write("Unmapped type '", classType->GetContained()->GetName().c_str(), "'");
                return COM::kError;
            }
            *out = (*hashIt).second;
            return COM::kOK;
        }
        case Data::NyShader::VectorType::kCID: {
            auto vec = static_cast<Data::NyShader::VectorType *>(type);
            llvm::Type *contained;
            if (!GetType(vec->type, &contained)) {
                return COM::kError;
            }
            *out = llvm::VectorType::get(contained, vec->x);
            return COM::kOK;
        }
        case Data::NyShader::MatrixType::kCID: {
            auto vec = static_cast<Data::NyShader::MatrixType *>(type);
            llvm::Type *contained;
            if (!GetType(vec->type, &contained)) {
                return COM::kError;
            }
            *out = llvm::ArrayType::get(llvm::VectorType::get(contained, vec->x), vec->y);
            return COM::kOK;
        }
        case Data::NyShader::TextureResourceType::kCID: {
            *out = llvm::IntegerType::getInt32Ty(manager->getContext());
            return COM::kOK;
        }
        case Data::NyShader::SamplerResourceType::kCID: {
            *out = llvm::IntegerType::getInt32Ty(manager->getContext());
            return COM::kOK;
        }
        default: {
            NyLang::PrettyStream stream;
            NyLang::IL::PrettyPrintContext context;
            type->PrettyPrint(stream, context);
            ErrorLog(registry).Write("Invalid or unsupported type found in NIL'", stream.GetData().c_str(), "'");
            return COM::kError;
        }
    }
    Always("Invalid base classid");
    return COM::kError;
}

COM::Result GlassCompiler::GetValue(NyLang::IL::IValue *value, llvm::BasicBlock *block, llvm::Value **out) {
    Always(value != nullptr);

    auto packed = packedValueMap.Find(value);
    if (packed != packedValueMap.End()) {
        llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
        gla::Builder glassBuilder(builder, manager, **metaData);
        builder.SetInsertPoint(block);

        *out = builder.CreateGEP(packed->source, std::vector<llvm::Value *>{
                llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), 0),
                llvm::ConstantInt::get(llvm::IntegerType::get(manager->getContext(), 32), packed->index)
        });
        return COM::kOK;
    }

    auto it = valueMap.Find(value);
    if (it == valueMap.end()) {
        switch (value->GetClassID()) {
            case NyLang::IL::ConstantIntValue::kCID: {
                *out = llvm::ConstantInt::get(
                        llvm::IntegerType::get(manager->getContext(), static_cast<UInt32>(value->GetType()->GetBitWidth())),
                        static_cast<UInt64>(static_cast<NyLang::IL::ConstantIntValue *>(value)->GetValue())
                );
                return COM::kOK;
            }
            case NyLang::IL::ConstantUIntValue::kCID: {
                *out = llvm::ConstantInt::get(
                        llvm::IntegerType::get(manager->getContext(), static_cast<UInt32>(value->GetType()->GetBitWidth())),
                        static_cast<UInt64>(static_cast<NyLang::IL::ConstantUIntValue *>(value)->GetValue())
                );
                return COM::kOK;
            }
            case NyLang::IL::ConstantFloatValue::kCID: {
                *out = llvm::ConstantFP::get(
                        llvm::Type::getFloatTy(manager->getContext()),
                        static_cast<NyLang::IL::ConstantFloatValue *>(value)->GetValue()
                );
                return COM::kOK;
            }
            case NyLang::IL::ConstantDoubleValue::kCID: {
                *out = llvm::ConstantFP::get(
                        llvm::Type::getDoubleTy(manager->getContext()),
                        static_cast<NyLang::IL::ConstantDoubleValue *>(value)->GetValue()
                );
                return COM::kOK;
            }
            case NyLang::IL::ConstantArrayValue::kCID: {
                Core::Array<llvm::Constant *> values;
                for (auto &&ilElement : static_cast<NyLang::IL::ConstantArrayValue *>(value)->GetValues()) {
                    llvm::Value *element;
                    if (!GetValue(ilElement, block, &element)) {
                        return COM::kError;
                    }

                    values.Add(llvm::cast<llvm::Constant>(element));
                }

                llvm::Type *contained;
                if (!GetType(static_cast<NyLang::IL::ConstantArrayValue *>(value)->GetContained(), &contained)) {
                    return COM::kError;
                }

                *out = llvm::ConstantArray::get(
                        llvm::ArrayType::get(contained, values.Size()),
                        values.handle
                );
                return COM::kOK;
            }
            case NyLang::IL::ConstantFunctionValue::kCID: {
                *out = functionMap[static_cast<NyLang::IL::ConstantFunctionValue *>(value)->GetFunction()];
                if (!*out) {
                    NyLang::PrettyStream stream;
                    NyLang::IL::PrettyPrintContext context;
                    value->PrettyPrint(stream, context);
                    ErrorLog(registry).Write("Bad constant function found in NIL'", stream.GetData().c_str(), "'");
                    return COM::kError;
                }
                return COM::kOK;
            }
            default: {
                NyLang::PrettyStream stream;
                NyLang::IL::PrettyPrintContext context;
                value->PrettyPrint(stream, context);
                ErrorLog(registry).Write("Invalid or unsupported value found in NIL '", stream.GetData().c_str(), "'");
                return COM::kError;
            }
        }
    }

    *out = (*it).second;

    auto proxyIt = proxyValueMap.Find(*out);
    if (proxyIt != proxyValueMap.End()) {
        if (!ExpandValueProxy(block, (*proxyIt).second, out)) {
            return COM::kError;
        }
    }

    return COM::kOK;
}

UInt32 GlassCompiler::GetLocation(UInt32 _register, UInt32 set) {
    return ((set + 1) << 16) | _register;
}

llvm::Value *GlassCompiler::GetTypeProxy(llvm::Type *type, const Core::String &name) {
    //auto var = new llvm::GlobalVariable(*manager->getModule(), type, true, llvm::GlobalVariable::ExternalLinkage, 0, (name).AsCharArray());
    auto var = llvm::ConstantPointerNull::get(llvm::PointerType::get(type, 0));
    return var;
}

Core::String GlassCompiler::SanitizeName(const Core::String &str) {
    Core::String shadow = str;
    shadow.Replace('.', '_');
    shadow.Replace('<', '_');
    shadow.Replace('>', '_');
    return shadow;
}

COM::Result GlassCompiler::GetBuiltin(const AnnotationChain &chain, bool isInput, gla::EMdBuiltIn &out) {
    out = gla::EMdBuiltIn::EmbNone;

    // Find builtin if needed
    for (int i = chain.index - 1; i >= 0; i--) {
        if (chain.annotations[i]->GetName() == "Semantic") {
            auto name = chain.annotations[i]->GetMemberWithName("name");
            if (!name) {
                ErrorLog(registry).Write("Semantic annotation does not have a name member");
                return COM::kError;
            }

            auto strVal = NyLang::OuterCast<NyLang::IL::ConstantStringValue>(name);
            if (!strVal) {
                ErrorLog(registry).Write("Semantic name not a string");
                return COM::kError;
            }

            // ...
            if (strVal->GetString() == "position") {
                if (isInput) {
                    out = gla::EMdBuiltIn::EmbCount;
                } else {
                    out = gla::EMdBuiltIn::EmbPosition;
                }
            } else if (strVal->GetString() == "vertexid") {
                out = gla::EMdBuiltIn::EmbVertexIndex;
            } else if (strVal->GetString() == "instanceid") {
                if (isInput) {
                    out = gla::EMdBuiltIn::EmbInstanceIndex;
                } else {
                    out = gla::EMdBuiltIn::EmbCount;
                }
            } else {
                // Unknown semantic
                ErrorLog(registry).Write("Unknown semantic name: '", strVal->GetString().c_str(), "'");
                return COM::kError;
            }
        }
    }
    return COM::kOK;
}

Core::String GlassCompiler::Unique(const char *str) {
    auto val = SanitizeName(str);
    UInt32 &count = uniqueMap[val];
    if (count++ > 0) {
        return val + count;
    }
    return val;
}

COM::Result GlassCompiler::ExpandValueProxy(llvm::BasicBlock *block, const GlassCompiler::ValueProxy &proxy, llvm::Value **out) {
    llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
    gla::Builder glassBuilder(builder, manager, **metaData);
    builder.SetInsertPoint(block);

    llvm::Value *container = builder.CreateAlloca(proxy.classType);

    for (USize i = 0; i < proxy.values.Size(); i++) {
        llvm::Value *value = proxy.values[i];

        auto it = proxyValueMap.Find(value);
        if (it != proxyValueMap.End()) {
            if (!ExpandValueProxy(block, (*it).second, &value)) {
                return COM::kError;
            }
        }

        builder.CreateStore(builder.CreateLoad(value), builder.CreateStructGEP(container, static_cast<UInt32>(i)));
    }

    *out = container;
    return COM::kOK;
}

COM::Result GlassCompiler::FlattenExtern(Vulkan::CompiledShaderBlob *blob, NyLang::IL::IType *, NyLang::IL::IValue *value, AnnotationChain chain, bool isVisible, llvm::Value **out, const Core::String &decoration,
                                         const Core::String &baseName) {
    // Convert IL type
    llvm::Type *type;
    if (!GetType(value->GetType(), &type)) {
        return COM::kError;
    }

    // Annotation chain
    for (auto &&annot : value->GetAnnotations()) {
        if (auto meta = NyLang::OuterCast<NyLang::IL::ConstantMetaValue>(annot)) {
            chain.Add(meta);
        }
    }

    // Class?
    if (auto _class = NyLang::OuterCast<NyLang::IL::ClassType>(value->GetType())) {
        *out = llvm::ConstantInt::get(llvm::Type::getInt32Ty(manager->getContext()), lookupRegister++);

        ValueProxy proxy;
        proxy.classType = type;

        // Flatten all members.
        UInt32 index = 0;
        for (auto &member : _class->GetContained()->GetMembers()) {
            llvm::Value *lookup;
            if (!FlattenExtern(
                    blob,
                    member->GetType(),
                    member,
                    chain,
                    isVisible,
                    &lookup,
                    Unique((decoration + "_" + member->GetName().c_str()).AsCharArray()),
                    baseName
            )) {
                return COM::kError;
            }
            index++;

            proxy.values.Add(lookup);
        }

        valueMap[value] = *out;
        proxyValueMap[*out] = proxy;
    } else {
        llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(manager->getContext());
        gla::Builder glaBuilder(builder, manager, **metaData);

        // Type
        auto valueType = value->GetType();

        // ...
        UInt32 arrayCount = 0;
        if (auto array = NyLang::OuterCast<NyLang::IL::ArrayType>(value->GetType())) {
            arrayCount = static_cast<UInt32>(array->GetCount());
            valueType = array->GetContained();
        }

        // Compiled descriptor
        GHA::PipelineScheduler::CompiledDescriptor compiledDescriptor;
        compiledDescriptor.name = baseName;
        compiledDescriptor.count = arrayCount;
        if (!GetInput(chain, baseName, compiledDescriptor)) {
            return COM::kError;
        }

        if (isVisible) {
            // Determine type of external
            switch (valueType->GetClassID()) {
                case Data::NyShader::TextureResourceType::kCID: {
                    auto resourceType = static_cast<Data::NyShader::TextureResourceType *>(valueType);

                    compiledDescriptor.rid = PipelineScheduler::DescriptorType::eTextureView;

                    llvm::Value *var = glaBuilder.createVariable(
                            gla::Builder::EStorageQualifier::ESQUniform,
                            0, type, nullptr, nullptr, decoration.AsCharArray()
                    );
                    //valueMap[value] = var;
                    *out = var;

                    // Create proxy
                    llvm::Value *proxyValue = GetTypeProxy(type, decoration + "_type");

                    // Determine sampler
                    gla::EMdSampler SamplerType = gla::EMdSampler::EMsTexture;
                    /*switch (((Data::NyShader::VectorType*)resourceType->value)->type->GetClassID()) {
                        case NyLang::IL::FPType::kCID: {
                            switch (((Data::NyShader::VectorType*)resourceType->value)->x) {
                                case 1:
                                    SamplerType = gla::EMdSampler::EMs;
                                    break;
                                case 2:
                                    SamplerType = gla::EMdSampler::;
                                    break;
                                case 3:
                                    SamplerType = gla::EMdSampler::;
                                    break;
                                case 4:
                                    SamplerType = gla::EMdSampler::;
                                    break;
                            }
                        }
                        case NyLang::IL::IntegerType::kCID: {

                        }
                    }*/

                    // Determine dimension
                    gla::EMdSamplerDim dim = {};
                    if (resourceType->type == Data::NyShader::TextureType::eBuffer) {
                        dim = gla::EMdSamplerDim::EMsdBuffer;
                    } else {
                        switch (resourceType->dim) {
                            case 1:
                                dim = gla::EMdSamplerDim::EMsd1D;
                                break;
                            case 2:
                                if (resourceType->type == Data::NyShader::TextureType::eCube) {
                                    dim = gla::EMdSamplerDim::EMsdCube;
                                } else {
                                    dim = gla::EMdSamplerDim::EMsd2D;
                                }
                                break;
                            case 3:
                                dim = gla::EMdSamplerDim::EMsd3D;
                                break;
                        }
                    }

                    // Set metadata
                    metaData->makeMdInputOutput(
                            decoration.AsCharArray(), gla::UniformListMdName, gla::EMdInputOutput::EMioDefaultUniform,
                            proxyValue, gla::EMdTypeLayout::EMtlSampler, gla::EMdPrecision::EMpNone, 1024,
                            metaData->makeMdSampler(
                                    SamplerType, proxyValue, dim, false,
                                    resourceType->isShadowMap, gla::EMdSamplerBaseType::EMsbFloat
                            ),
                            nullptr,
                            -1,
                            gla::EMdBuiltIn::EmbNone,
                            GetLocation(compiledDescriptor.index, compiledDescriptor.set)
                    );
                    break;
                }
                default:
                    ErrorLog(registry).Write("Unknown external type in NIL");
                    return COM::kError;
            }

            // Add resource
            blob->resources[compiledDescriptor.set].Add(compiledDescriptor);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result GlassCompiler::GetInput(const AnnotationChain &chain, const Core::String& name, GHA::PipelineScheduler::CompiledDescriptor &out) {
    // Find input
    for (int i = chain.index - 1; i >= 0; i--) {
        if (chain.annotations[i]->GetName() == "Resource") {
            auto bucket = chain.annotations[i]->GetMemberWithName("bucket");
            if (!bucket) {
                ErrorLog(registry).Write("Resource annotation does not have a bucket member");
                return COM::kError;
            }

            auto intVal = NyLang::OuterCast<NyLang::IL::ConstantIntValue>(bucket);
            if (!intVal) {
                ErrorLog(registry).Write("Resource bucket not an integer");
                return COM::kError;
            }

            auto id = chain.annotations[i]->GetMemberWithName("id");
            if (!id) {
                ErrorLog(this->registry).Write("Resource annotation does not have an id member");
                return COM::kError;
            }

            auto idStrVal = NyLang::OuterCast<NyLang::IL::ConstantStringValue>(id);
            if (!idStrVal) {
                ErrorLog(this->registry).Write("Resource id not a string");
                return COM::kError;
            }

            // Automatic binding?
            if (idStrVal->GetString() != "") {
                GHA::ShaderBinding binding;
                binding.target = GHA::BindID(Core::ComputeCRC64(name.AsCharArray()));
                binding.id = GHA::BindID(Core::ComputeCRC64(idStrVal->GetString().c_str()));
                binding.name = idStrVal->GetString().c_str();
                shaderBlob->bindings.Add(binding);
            }

            out.set = static_cast<UInt32>(intVal->GetValue());
            out.index = registerOffsets[out.set]++;
            return COM::kOK;
        }
    }

    // ...
    return COM::kError;
}

COM::Result GlassCompiler::GetAggregateMD(NyLang::IL::Class *container, llvm::MDNode *layout, llvm::MDNode **out, const Core::String &decoration) {
    // Aggregate layout:
    //  !aggregate -> { name, !typeLayout, list of members: name1, !aggregate1, name2, !aggregate2, ... }

    // ...
    /*if (*out = aggregateMDs[container]) {
        return COM::kOK;
    }*/

    llvm::Type *type = typeMap[container];

    // Prepare header
    Core::Array<llvm::Value *> args;
    args.Add(llvm::MDString::get(manager->getContext(), type->getStructName()));
    args.Add(layout);

    // Members
    for (auto &member : container->GetMembers()) {
        llvm::Type *type;
        if (!GetType(member->GetType(), &type)) {
            return COM::kError;
        }

        if (type->isStructTy()) {
            args.Add(llvm::MDString::get(manager->getContext(), SanitizeName(member->GetName().c_str()).AsCharArray()));

            llvm::MDNode *md;
            if (!GetAggregateMD(
                    NyLang::OuterCast<NyLang::IL::ClassType>(member->GetType())->GetContained(),
                    metaData->makeMdTypeLayout(
                            gla::EMdTypeLayout::EMtlNone,
                            gla::EMdPrecision::EMpNone,
                            gla::MaxUserLayoutLocation,
                            nullptr
                    ),
                    &md,
                    member->GetName().c_str()
            )) {
                return COM::kError;
            }
            args.Add(md);
        } else if (type->isArrayTy() && type->getContainedType(0)->isStructTy()) {
            args.Add(llvm::MDString::get(manager->getContext(), SanitizeName(member->GetName().c_str()).AsCharArray()));

            llvm::MDNode *md;
            if (!GetAggregateMD(
                    NyLang::OuterCast<NyLang::IL::ClassType>(NyLang::OuterCast<NyLang::IL::ArrayType>(member->GetType())->GetContained())->GetContained(),
                    metaData->makeMdTypeLayout(
                            gla::EMdTypeLayout::EMtlNone,
                            gla::EMdPrecision::EMpNone,
                            gla::MaxUserLayoutLocation,
                            nullptr
                    ),
                    &md,
                    SanitizeName(member->GetName().c_str()).AsCharArray()
            )) {
                return COM::kError;
            }
            args.Add(md);
        } else {
            // Determine type layout
            gla::EMdTypeLayout typeLayout = gla::EMdTypeLayout::EMtlNone;
            if (member->GetType()->GetClassID() == Data::NyShader::MatrixType::kCID) {
                if (type->isAggregateType() && type->getContainedType(0)->isVectorTy())
                    typeLayout = gla::EMdTypeLayout::EMtlColMajorMatrix;
                if (type->isAggregateType() && type->getContainedType(0)->isAggregateType() && type->getContainedType(0)->getContainedType(0)->isVectorTy())
                    typeLayout = gla::EMdTypeLayout::EMtlColMajorMatrix;
            } else if (auto intType = NyLang::OuterCast<NyLang::IL::IntegerType>(member->GetType())) {
                if (intType->IsUnsigned()) {
                    typeLayout = gla::EMdTypeLayout::EMtlUnsigned;
                }
            }

            args.Add(llvm::MDString::get(manager->getContext(), SanitizeName(member->GetName().c_str()).AsCharArray()));

            args.Add(llvm::MDNode::get(
                    manager->getContext(),
                    std::vector<llvm::Value *>{
                            llvm::MDString::get(manager->getContext(), SanitizeName(member->GetName().c_str()).AsCharArray()),
                            metaData->makeMdTypeLayout(
                                    typeLayout,
                                    gla::EMdPrecision::EMpNone,
                                    gla::MaxUserLayoutLocation,
                                    nullptr
                            )
                    }
            ));
        }
    }

    // Compile nodes
    *out = (aggregateMDs[container] = llvm::MDNode::get(manager->getContext(), args.handle));

    // OK
    return COM::kOK;
}
