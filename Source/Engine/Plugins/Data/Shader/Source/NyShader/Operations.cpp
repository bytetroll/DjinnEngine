#include <Data/NyShader/Operations.h>
#include <Core/Common.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Diagnostic/IDiagnostic.h>
#include <NyLang/Frontend/Semantic/SemanticContext.h>
#include <NyLang/Core/Host/Registry.h>
#include <NyLang/Native/Extensions/BuiltinExtension.h>
#include <NyLang/Native/Extensions/UtilExtension.h>
#include <NyLang/Native/Symbols/SymbolType.h>
#include <NyLang/Native/Symbols/SymbolValue.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeInteger.h>
#include <NyLang/Frontend/Node/Node.h>
#include <NyLang/Native/Messages/NativeSemanticMessages.h>
#include <NyLang/Native/Symbols/Values/SymbolValueString.h>
#include <NyLang/Native/Symbols/Values/SymbolValueInteger.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeArray.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeFP.h>
#include <NyLang/Frontend/Compiler/CompilerContext.h>
#include <NyLang/Frontend/Semantic/SemanticContext.h>
#include <NyLang/Backend/IL/Program.h>
#include <NyLang/Backend/IL/IType.h>
#include <NyLang/Backend/IL/IInstruction.h>
#include <NyLang/Native/IL/NativeEmitter.h>
#include <NyLang/Backend/Pass/PassManager.h>
#include <NyLang/Native/Passes/DeadIRPass.h>
#include <Data/NyShader/Types.h>
#include <Data/NyShader/NILTypes.h>
#include <Data/NyShader/NILInstructions.h>
#include <NyLang/Frontend/Parser/IParser.h>
#include <NyLang/Frontend/Parser/ParserContext.h>
#include <NyLang/Frontend/Tokenizer/ITokenizer.h>
#include <NyLang/Frontend/Semantic/ISemantic.h>
#include <NyLang/Diagnostic/DiagnosticScope.h>
#include <Core/String.h>
#include <Platform/FileSystem.h>
#include <NyLang/Frontend/Compiler/ICompiler.h>
#include <NyLang/Native/IL/Values/ConstantIntValue.h>
#include <NyLang/Native/IL/Types/IntegerType.h>
#include <NyLang/Native/IL/Types/FPType.h>
#include <Base/DefaultLogs.h>
#include <NyLang/Native/IL/Values/ConstantFloatValue.h>
#include <NyLang/Native/IL/Values/ConstantDoubleValue.h>
#include <Base/IFileSystem.h>
#include <Base/FileHandle.h>
#include <Core/StringStream.h>

DJINN_NS3(Data, NyShader);

namespace Djinn::Data::NyShader {
    /// Compiler messages.
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedTexture, NyLang::MessageSeverity::eError, stream << "Expected texture value");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedSampler, NyLang::MessageSeverity::eError, stream << "Expected sampler value");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedDimensionInteger, NyLang::MessageSeverity::eError, stream << "Expected dimension integer value");
    LANG_SIMPLEMESSAGE(SemanticMessageInvalidTextureType, NyLang::MessageSeverity::eError, stream << "Invalid texture type");
    LANG_SIMPLEMESSAGE(SemanticMessageVectorTypeMismatch, NyLang::MessageSeverity::eError, stream << "Vector components must be of the same type");
    LANG_SIMPLEMESSAGE(SemanticMessageMatrixTypeMismatch, NyLang::MessageSeverity::eError, stream << "Matrix components must be of the same type");
    LANG_SIMPLEMESSAGE(SemanticMessageExpectedMatrix, MessageSeverity::eError, stream << "Expected matrix value");
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageVectorAssignMismatch, NyLang::MessageSeverity::eError, NyLang::SymbolTypeInfo *, from, NyLang::SymbolTypeInfo *, to, {
        stream << "Cannot assign vector of type ";
        from->PrettyPrint(stream);
        stream << " to ";
        to->PrettyPrint(stream);
    });
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageIncludeScope, NyLang::MessageSeverity::eError, NyLang::String, filename, {
        stream << "Inside included file: '" << filename << "'";
    });
    LANG_SIMPLEMESSAGE_1ARG(SemanticMessageFileNotFound, NyLang::MessageSeverity::eError, NyLang::String, filename, {
        stream << "Could not find file: '" << filename << "'";
    });
    LANG_SIMPLEMESSAGE_2ARG(SemanticMessageMatrixAssignMismatch, NyLang::MessageSeverity::eError, NyLang::SymbolTypeInfo *, from, NyLang::SymbolTypeInfo *, to, {
        stream << "Cannot assign matrix of type ";
        from->PrettyPrint(stream);
        stream << " to ";
        to->PrettyPrint(stream);
    });
}

ShaderContext::ShaderContext() {

}

NyLang::ClassID ShaderContext::GetClassID() {
    return kCID;
}

void ShaderContext::Reset() {
    paths.Clear();
}

COM::Result NyShader::InstallOperations(Host::IRegistry *_registry, NyLang::Registry *registry) {
    using namespace NyLang;

    // Get file system
    Base::IFileSystem* fs = nullptr;
    if (!_registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(_registry).Write("Failed to install ny operations, expected file system to be installed");
        return COM::kError;
    }

    // Get builtin extension
    auto builtinExtension = registry->GetClass<BuiltinExtension>();
    auto utilExtension = registry->GetClass<UtilExtension>();
    auto compiler = registry->GetInterface<ICompiler>();
    auto tokenizer = compiler->GetTokenizer();
    auto parser = compiler->GetParser();
    auto semantic = compiler->GetSemantic();
    if (!builtinExtension || !utilExtension || !compiler || !tokenizer || !parser || !semantic) {
        ErrorLog(_registry).Write("Failed to install ny operations, missing components");
        return COM::kError;
    }

    // Install inbuilts
    auto shaderContext = registry->context.New<ShaderContext>();
    registry->AddClass(shaderContext);

    // Include builtin
    builtinExtension->AddBuiltin("__include", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be a string value
        auto val = args[0]->GetComponent<SymbolValueInfo>();
        if (!val || (val->value->GetClassID() != NyLang::SymbolValueString::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedString>(node->tok));
            return nullptr;
        }

        // Get filename
        const char *file = OuterCast<SymbolValueString>(val->value)->value.c_str();

        // Already contains
        if (shaderContext->paths.Contains(file)) {
            return nullptr;
        }
        shaderContext->paths.Add(file);

        Base::FileHandle handle;

        // Attempt to locate file
        Core::String contents;
        if (fs->OpenStream(Base::ToPath(Core::String("Runtime/Engine/Shaders/") + file), Platform::FileMode::eRead, &handle)) {
            contents = Core::StringStream(handle).ReadAll();
        } else if (Platform::FileSystem::LoadTextFile(file, contents)) {

        } else {
            context->diagnostic->Queue(registry->context.New<SemanticMessageFileNotFound>(node->tok, file));
            return nullptr;
        }

        // Add diagnostic scope
        auto &&scope = DiagnosticScope(context->diagnostic, registry->context.New<SemanticMessageIncludeScope>(node->tok, file));

        // Tokenize
        auto tokContext = tokenizer->Tokenize(contents.AsCharArray());
        if (!tokContext) {
            return nullptr;
        }

        // Parse
        auto parseContext = parser->ParseExtern(tokContext);
        if (!parseContext) {
            return nullptr;
        }

        // Move to result
        node->operands.push_back(parseContext->root);
        node->Move({}, parseContext->root);

        // Visit late
        SemanticEvent event;
        if (!(event = semantic->Visit(context, parseContext->root, scopes - "local"_sscope))) {
            return nullptr;
        }
        return nullptr;
    });

    // Vector assignment
    utilExtension->AddAssignOp(NyShader::SymbolTypeVector::kCID, NyShader::SymbolTypeVector::kCID, [=](SemanticContext *context, Node *, const SemanticScopeStack &, const Token &,
                                                                                                       const Token &targetTok, SymbolValueInfo *target,
                                                                                                       const Token, SymbolValueInfo *value
    ) -> SymbolValueInfo * {
        if (!target->value->TypeInfo->type->Compare(value->value->TypeInfo->type)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorAssignMismatch>(targetTok, target->value->TypeInfo, value->value->TypeInfo));
            return nullptr;
        }
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);
        emitter.CreateStore(target->value->GetIL(registry->context), value->value->GetIL(registry->context));
        return target;
    });

    // Matrix assignment
    utilExtension->AddAssignOp(NyShader::SymbolTypeMatrix::kCID, NyShader::SymbolTypeMatrix::kCID, [=](SemanticContext *context, Node *, const SemanticScopeStack, const Token &,
                                                                                                       const Token &targetTok, SymbolValueInfo *target,
                                                                                                       const Token &, SymbolValueInfo *value
    ) -> SymbolValueInfo * {
        if (!target->value->TypeInfo->type->Compare(value->value->TypeInfo->type)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageMatrixAssignMismatch>(targetTok, target->value->TypeInfo, value->value->TypeInfo));
            return nullptr;
        }
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);
        emitter.CreateStore(target->value->GetIL(registry->context), value->value->GetIL(registry->context));
        return target;
    });

    // Vector type factor builtin
    builtinExtension->AddBuiltin("__dsf_vector", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting one argument
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be type
        auto type = args[0]->GetComponent<SymbolTypeInfo>();
        if (!type) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedType>(node->tok));
            return nullptr;
        }

        // Get count
        int count = 1;
        if (auto array = OuterCast<SymbolTypeArray>(type->type)) {
            type = array->ContainedTypeInfo;
            count = array->Count;
        }

        // To vector type
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTypeVector>(registry->context, type, count))
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_set", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1..4 arguments
        if (args.empty() || args.size() > 4) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 4, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        UInt32 count = 0;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            if (valueInfo->value->TypeInfo->type->GetClassID() == Data::NyShader::SymbolTypeVector::kCID) {
                type = OuterCast<Data::NyShader::SymbolTypeVector>(valueInfo->value->TypeInfo->type)->ContainedTypeInfo;
                count += OuterCast<Data::NyShader::SymbolTypeVector>(valueInfo->value->TypeInfo->type)->CountX;
            } else {
                type = valueInfo->value->TypeInfo;
                count++;
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTypeVector>(registry->context, type, count)),
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorSetInstruction>(
                                registry->context.New<NyShader::VectorType>(type->il, count),
                                values
                        ))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_zero", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 argument
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be type
        auto targetType = args[0]->GetComponent<SymbolTypeInfo>();
        if (!targetType) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedType>(node->tok));
            return nullptr;
        }

        auto vecType = OuterCast<NyShader::SymbolTypeVector>(targetType->type);
        if (!vecType) {
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        PtrVector<IL::IValue> values;
        for (UInt32 i = 0; i < vecType->CountX; i++) {
            switch (vecType->ContainedTypeInfo->type->il->GetClassID()) {
                case IL::IntegerType::kCID:
                    values.push_back(registry->context.New<IL::ConstantIntValue>(registry->context, OuterCast<IL::IntegerType>(vecType->ContainedTypeInfo->type->il)->GetBitWidth(), 0));
                    break;
                case IL::FPType::kCID:
                    if (OuterCast<IL::FPType>(vecType->ContainedTypeInfo->type->il)->GetBitWidth() == 32) {
                        values.push_back(registry->context.New<IL::ConstantFloatValue>(registry->context, 0));
                    } else {
                        values.push_back(registry->context.New<IL::ConstantDoubleValue>(registry->context, 0));
                    }
                    break;
            }
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context, targetType,
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorSetInstruction>(
                                registry->context.New<NyShader::VectorType>(vecType->ContainedTypeInfo->il, values.size()),
                                values
                        ))
                )
        });
    });

    // Vector swizzle builtin
    builtinExtension->AddBuiltin("__dsf_vector_swizzle", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2..5 arguments
        if (args.size() < 2 || args.size() > 5) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Argument 0 must be vector value
        auto vec = args[0]->GetComponent<SymbolValueInfo>();
        if (!vec || (vec->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        Core::FixedArray4<int> components;
        for (UInt32 i = 1; i < args.size(); i++) {
            // Argument must be integer
            auto value = args[i]->GetComponent<SymbolValueInfo>();
            if (!value || (value->value->GetClassID() != SymbolValueInteger::kCID)) {
                context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedDimensionInteger>(node->tok));
                return nullptr;
            }

            components.Add(OuterCast<SymbolValueInteger>(value->value)->value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        if (components.Size() == 1) {
            return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                    registry->context.New<SymbolValueInfo>(
                            registry->context,
                            OuterCast<SymbolTypeVector>(vec->value->TypeInfo->type)->ContainedTypeInfo,
                            IL::ValueAddressPair(emitter.Create<NyShader::VectorSwizzleInstruction>(
                                    OuterCast<SymbolTypeVector>(vec->value->TypeInfo->type)->ContainedTypeInfo->il,
                                    vec->value->GetIL(registry->context),
                                    components
                            ))
                    )
            });
        } else {
            return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                    registry->context.New<SymbolValueInfo>(
                            registry->context,
                            registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTypeVector>(registry->context, OuterCast<SymbolTypeVector>(
                                    vec->value->TypeInfo->type)->ContainedTypeInfo, components.Size())),
                            IL::ValueAddressPair(emitter.Create<NyShader::VectorSwizzleInstruction>(
                                    registry->context.New<NyShader::VectorType>(OuterCast<SymbolTypeVector>(vec->value->TypeInfo->type)->ContainedTypeInfo->il, components.Size()),
                                    vec->value->GetIL(registry->context),
                                    components
                            ))
                    )
            });
        }
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_add", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments.
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                /*if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }*/
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context, type,
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorAddInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_length", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments.
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be vector value
        auto vec = args[0]->GetComponent<SymbolValueInfo>();
        if (!vec || (vec->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<NyLang::SymbolTypeInfo>(registry->context, registry->context.New<NyLang::SymbolTypeFP>(registry->context)),
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorLengthInstruction>(registry->context, vec->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_length2", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments.
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be vector value
        auto vec = args[0]->GetComponent<SymbolValueInfo>();
        if (!vec || (vec->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<NyLang::SymbolTypeInfo>(registry->context, registry->context.New<NyLang::SymbolTypeFP>(registry->context)),
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorLength2Instruction>(registry->context, vec->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_sum", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments.
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be vector value
        auto vec = args[0]->GetComponent<SymbolValueInfo>();
        if (!vec || (vec->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<NyLang::SymbolTypeInfo>(registry->context, registry->context.New<NyLang::SymbolTypeFP>(registry->context)),
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorSumInstruction>(registry->context, vec->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_sub", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                /*if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }*/
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorSubInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin.
    builtinExtension->AddBuiltin("__dsf_vector_div", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                /*if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }*/
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorDivInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_mul", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                /*if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }*/
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::VectorMulInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector type factor builtin
    builtinExtension->AddBuiltin("__dsf_matrix", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting one argument
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be type
        auto type = args[0]->GetComponent<SymbolTypeInfo>();
        if (!type) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedType>(node->tok));
            return nullptr;
        }

        // Get counts
        int countX = 1;
        int countY = 1;
        if (auto array = OuterCast<SymbolTypeArray>(type->type)) {
            if (auto arrayY = OuterCast<SymbolTypeArray>(array->ContainedTypeInfo->type)) {
                type = arrayY->ContainedTypeInfo;
                countX = arrayY->Count;
                countY = array->Count;
            }
        }

        // To matrix type
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTypeMatrix>(registry->context, type, countX, countY))
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_matrix_set", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting at least 1 argument
        if (args.empty()) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be type
        auto targetType = args[0]->GetComponent<SymbolTypeInfo>();
        if (!targetType) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedType>(node->tok));
            return nullptr;
        }

        // Collect arguments
        PtrVector<IL::IValue> values;
        for (auto arg = ++args.begin(); arg != args.end(); ++arg) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, *arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        targetType,
                        IL::ValueAddressPair(emitter.Create<NyShader::MatrixSetInstruction>(
                                targetType->il,
                                values
                        ))
                )
        });
    });

    // Matrix addition builtin
    builtinExtension->AddBuiltin("__dsf_matrix_zero", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 argument
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Argument 0 must be type
        auto targetType = args[0]->GetComponent<SymbolTypeInfo>();
        if (!targetType) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedType>(node->tok));
            return nullptr;
        }

        auto matType = OuterCast<NyShader::SymbolTypeMatrix>(targetType->type);
        if (!matType) {
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        PtrVector<IL::IValue> values;
        for (UInt32 i = 0; i < (matType->CountX * matType->CountY); i++) {
            switch (matType->ContainedTypeInfo->type->il->GetClassID()) {
                case IL::IntegerType::kCID:
                    values.push_back(registry->context.New<IL::ConstantIntValue>(registry->context, OuterCast<IL::IntegerType>(matType->ContainedTypeInfo->type->il)->GetBitWidth(), 0));
                    break;
                case IL::FPType::kCID:
                    values.push_back(registry->context.New<IL::ConstantIntValue>(registry->context, OuterCast<IL::FPType>(matType->ContainedTypeInfo->type->il)->GetBitWidth(), 0));
                    break;
            }
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        targetType,
                        IL::ValueAddressPair(emitter.Create<NyShader::MatrixSetInstruction>(
                                registry->context.New<NyShader::MatrixType>(matType->ContainedTypeInfo->il, matType->CountX, matType->CountY),
                                values
                        ))
                )
        });
    });

    // Matrix multiplication builtin
    builtinExtension->AddBuiltin("__dsf_matrix_mul", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ...
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageMatrixTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::MatrixMulInstruction>(values[0], values[1]))
                )
        });
    });

    // Matrix transformation builtin
    builtinExtension->AddBuiltin("__dsf_matrix_transform", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Argument 0 must be matrix value
        auto mat = args[0]->GetComponent<SymbolValueInfo>();
        if (!mat || (mat->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeMatrix::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedMatrix>(node->tok));
            return nullptr;
        }

        // Argument 1 must be vector value
        auto vec = args[1]->GetComponent<SymbolValueInfo>();
        if (!vec || (vec->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        vec->value->TypeInfo,
                        IL::ValueAddressPair(emitter.Create<NyShader::MatrixTransformInstruction>(mat->value->GetIL(registry->context).value, vec->value->GetIL(registry->context).value))
                )
        });
    });

    // Texture types
    builtinExtension->AddBuiltin("__dsf_texture", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Needs 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Argument 0 must be string
        auto type = args[0]->GetComponent<SymbolValueInfo>();
        if (!type || (type->value->GetClassID() != SymbolValueString::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedString>(node->tok));
            return nullptr;
        }

        // ...
        auto typeStr = OuterCast<SymbolValueString>(type->value);

        // Convert type
        NyShader::TextureType textureType;
        if (typeStr->value == "buffer") {
            textureType = NyShader::TextureType::eBuffer;
        } else if (typeStr->value == "color") {
            textureType = NyShader::TextureType::eNone;
        } else if (typeStr->value == "cube") {
            textureType = NyShader::TextureType::eCube;
        } else {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageInvalidTextureType>(node->tok));
            return nullptr;
        }

        // Argument 1 must be type
        auto valueType = args[1]->GetComponent<SymbolTypeInfo>();
        if (!valueType || (valueType->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVectorType>(node->tok));
            return nullptr;
        }

        if (OuterCast<NyShader::SymbolTypeVector>(valueType->type)->CountX == 1) {
            valueType = OuterCast<NyShader::SymbolTypeVector>(valueType->type)->ContainedTypeInfo;
        }

        // Argument 2 must be integer
        auto value = args[2]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->GetClassID() != SymbolValueInteger::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedDimensionInteger>(node->tok));
            return nullptr;
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTextureType>(
                        registry->context, textureType,
                        valueType,
                        OuterCast<SymbolValueInteger>(value->value)->value
                ))
        });
    });

    // Texture types
    builtinExtension->AddBuiltin("__dsf_texture_map", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Needs 4 arguments
        if (args.size() != 4) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 4, args.size()));
            return nullptr;
        }

        // Argument 0 must be string
        auto type = args[0]->GetComponent<SymbolValueInfo>();
        if (!type || (type->value->GetClassID() != SymbolValueString::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedString>(node->tok));
            return nullptr;
        }

        // ...
        auto typeStr = OuterCast<SymbolValueString>(type->value);

        // Convert type
        NyShader::TextureType textureType;
        if (typeStr->value == "buffer") {
            textureType = NyShader::TextureType::eBuffer;
        } else if (typeStr->value == "color") {
            textureType = NyShader::TextureType::eNone;
        } else if (typeStr->value == "cube") {
            textureType = NyShader::TextureType::eCube;
        } else {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageInvalidTextureType>(node->tok));
            return nullptr;
        }

        // Argument 1 must be type
        auto valueType = args[1]->GetComponent<SymbolTypeInfo>();
        if (!valueType || (valueType->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVectorType>(node->tok));
            return nullptr;
        }

        if (OuterCast<NyShader::SymbolTypeVector>(valueType->type)->CountX == 1) {
            valueType = OuterCast<NyShader::SymbolTypeVector>(valueType->type)->ContainedTypeInfo;
        }

        // Argument 2 must be integer
        auto value = args[2]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->GetClassID() != SymbolValueInteger::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedDimensionInteger>(node->tok));
            return nullptr;
        }

        // Argument 3 must be integer
        auto count = args[3]->GetComponent<SymbolValueInfo>();
        if (!count || (count->value->GetClassID() != SymbolValueInteger::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedDimensionInteger>(node->tok));
            return nullptr;
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolTypeArray>(registry->context,
                                                                                                                          registry->context.New<NyShader::SymbolTypeInfo>(registry->context,
                                                                                                                                                                          registry->context.New<NyShader::SymbolTextureType>(
                                                                                                                                                                                  registry->context,
                                                                                                                                                                                  textureType,
                                                                                                                                                                                  valueType,
                                                                                                                                                                                  OuterCast<SymbolValueInteger>(
                                                                                                                                                                                          value->value)->value
                                                                                                                                                                          )),
                                                                                                                          OuterCast<SymbolValueInteger>(count->value)->value))
        });
    });

    // Sampler types
    builtinExtension->AddBuiltin("__dsf_sampler", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Needs 0 arguments
        if (args.size() != 0) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 0, args.size()));
            return nullptr;
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<NyShader::SymbolSamplerType>(registry->context))
        });
    });

    // Texture ops
    builtinExtension->AddBuiltin("__dsf_texture_samplegradient", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // Needs 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Argument 0 must be texture value
        auto value = args[0]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->TypeInfo->type->GetClassID() != NyShader::SymbolTextureType::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedTexture>(node->tok));
            return nullptr;
        }
        auto textureType = OuterCast<NyShader::SymbolTextureType>(value->value->TypeInfo->type);

        // Argument 1 must be vector value.
        auto loc = args[1]->GetComponent<SymbolValueInfo>();
        if (!loc || (loc->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        textureType->value,
                        IL::ValueAddressPair(emitter.Create<NyShader::TextureSampleGradientInstruction>(value->value->GetIL(registry->context).value, textureType->value->il,
                                                                                                        loc->value->GetIL(registry->context).value))
                )
        });
    });

    // Texture ops
    builtinExtension->AddBuiltin("__dsf_texture_map_samplegradient", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // Needs 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Argument 0 must be array value
        auto value = args[0]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeArray::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedTexture>(node->tok));
            return nullptr;
        }
        auto arrayType = OuterCast<NyShader::SymbolTypeArray>(value->value->TypeInfo->type);
        auto textureType = OuterCast<NyShader::SymbolTextureType>(arrayType->ContainedTypeInfo->type);

        // Argument 1 must be vector value.
        auto loc = args[1]->GetComponent<SymbolValueInfo>();
        if (!loc || (loc->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // Argument 2 must be int value.
        auto index = args[2]->GetComponent<SymbolValueInfo>();
        if (!index || (index->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeInteger::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // Load texture
        auto texture = emitter.CreateElement(value->value->GetIL(registry->context), index->value->GetIL(registry->context));

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        textureType->value,
                        IL::ValueAddressPair(emitter.Create<NyShader::TextureSampleGradientInstruction>(texture.value, textureType->value->il, loc->value->GetIL(registry->context).value))
                )
        });
    });

    // Texture ops
    builtinExtension->AddBuiltin("__dsf_texture_sampletexel", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // Needs 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Argument 0 must be texture value
        auto value = args[0]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->TypeInfo->type->GetClassID() != NyShader::SymbolTextureType::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedTexture>(node->tok));
            return nullptr;
        }
        auto textureType = OuterCast<NyShader::SymbolTextureType>(value->value->TypeInfo->type);

        // Argument 2 must be vector value
        auto loc = args[1]->GetComponent<SymbolValueInfo>();
        if (!loc || (loc->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        textureType->value,
                        IL::ValueAddressPair(emitter.Create<NyShader::TextureSampleTexelInstruction>(value->value->GetIL(registry->context).value, textureType->value->il,
                                                                                                     loc->value->GetIL(registry->context).value))
                )
        });
    });

    // Texture ops
    builtinExtension->AddBuiltin("__dsf_texture_map_sampletexel", [=](SemanticContext *context, Node *node, const SemanticScopeStack &, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // Needs 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Argument 0 must be array value
        auto value = args[0]->GetComponent<SymbolValueInfo>();
        if (!value || (value->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeArray::kCID)) {
            context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageExpectedTexture>(node->tok));
            return nullptr;
        }
        auto arrayType = OuterCast<NyShader::SymbolTypeArray>(value->value->TypeInfo->type);
        auto textureType = OuterCast<NyShader::SymbolTextureType>(arrayType->ContainedTypeInfo->type);

        // Argument 2 must be vector value
        auto loc = args[1]->GetComponent<SymbolValueInfo>();
        if (!loc || (loc->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeVector::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // Argument 3 must be int value.
        auto index = args[2]->GetComponent<SymbolValueInfo>();
        if (!index || (index->value->TypeInfo->type->GetClassID() != NyShader::SymbolTypeInteger::kCID)) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedVector>(node->tok));
            return nullptr;
        }

        // Load texture
        auto texture = emitter.CreateElement(value->value->GetIL(registry->context), index->value->GetIL(registry->context));

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        textureType->value,
                        IL::ValueAddressPair(emitter.Create<NyShader::TextureSampleTexelInstruction>(texture.value, textureType->value->il, loc->value->GetIL(registry->context).value))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_smoothstep", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::SmoothstepInstruction>(values[0], values[1], values[2]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_clamp", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::ClampInstruction>(values[0], values[1], values[2]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_max", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::MaxInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_min", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::MinInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_abs", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Must be value.
        auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, args[0]);
        if (!valueInfo) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
            return nullptr;
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        valueInfo->value->TypeInfo,
                        IL::ValueAddressPair(emitter.Create<NyShader::AbsInstruction>(valueInfo->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_sqrt", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Must be value.
        auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, args[0]);
        if (!valueInfo) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
            return nullptr;
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        valueInfo->value->TypeInfo,
                        IL::ValueAddressPair(emitter.Create<NyShader::SqrtInstruction>(valueInfo->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_asfloat", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Must be value.
        auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, args[0]);
        if (!valueInfo) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
            return nullptr;
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<SymbolTypeFP>(registry->context, 32)),
                        IL::ValueAddressPair(emitter.Create<NyShader::AsFloatInstruction>(registry->context, valueInfo->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_asuint", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Must be value.
        auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, args[0]);
        if (!valueInfo) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
            return nullptr;
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<SymbolTypeInteger>(registry->context, true, 32)),
                        IL::ValueAddressPair(emitter.Create<NyShader::AsUIntInstruction>(registry->context, valueInfo->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_asint", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 1 arguments
        if (args.size() != 1) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 1, args.size()));
            return nullptr;
        }

        // Must be value.
        auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, args[0]);
        if (!valueInfo) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
            return nullptr;
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<SymbolTypeInfo>(registry->context, registry->context.New<SymbolTypeInteger>(registry->context, false, 32)),
                        IL::ValueAddressPair(emitter.Create<NyShader::AsIntInstruction>(registry->context, valueInfo->value->GetIL(registry->context)))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_lerp", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 3 arguments
        if (args.size() != 3) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 3, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value.
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            if (!type) {
                type = valueInfo->value->TypeInfo;
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::LerpInstruction>(values[0], values[1], values[2]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_dot", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<NyLang::SymbolTypeInfo>(registry->context, registry->context.New<NyLang::SymbolTypeFP>(registry->context)),
                        IL::ValueAddressPair(emitter.Create<NyShader::DotInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_distance", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ..
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        registry->context.New<NyLang::SymbolTypeInfo>(registry->context, registry->context.New<NyLang::SymbolTypeFP>(registry->context)),
                        IL::ValueAddressPair(emitter.Create<NyShader::DistanceInstruction>(values[0], values[1]))
                )
        });
    });

    // Vector addition builtin
    builtinExtension->AddBuiltin("__dsf_vector_cross", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<Symbol *> &args) -> Symbol * {
        // Expecting 2 arguments
        if (args.size() != 2) {
            context->diagnostic->Queue(registry->context.New<SemanticMessageArgumentCountMismatch>(node->tok, 2, args.size()));
            return nullptr;
        }

        // Collect arguments
        SymbolTypeInfo* type = nullptr;
        PtrVector<IL::IValue> values;
        for (auto &&arg : args) {
            // Must be value
            auto valueInfo = utilExtension->ResolveValueInfo(context, node, scopes, arg);
            if (!valueInfo) {
                context->diagnostic->Queue(registry->context.New<SemanticMessageExpectedValue>(node->tok));
                return nullptr;
            }

            // ..
            if (!type) {
                type = valueInfo->value->TypeInfo;
            } else {
                if (!type->Compare(valueInfo->value->TypeInfo)) {
                    context->diagnostic->Queue(registry->context.New<NyShader::SemanticMessageVectorTypeMismatch>(node->tok));
                    return nullptr;
                }
            }

            // ...
            values.push_back(valueInfo->value->GetIL(registry->context).value);
        }

        // ..
        IL::NativeEmitter emitter(registry->context, context->ilEmitter);

        // ...
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context,
                        type,
                        IL::ValueAddressPair(emitter.Create<NyShader::CrossInstruction>(values[0], values[1]))
                )
        });
    });

    // OK
    return COM::kOK;
}
