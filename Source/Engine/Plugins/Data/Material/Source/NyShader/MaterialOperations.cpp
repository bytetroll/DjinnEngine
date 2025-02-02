#include <Data/NyShader/MaterialOperations.h>
#include <Core/Common.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Diagnostic/IDiagnostic.h>
#include <NyLang/Frontend/Semantic/SemanticContext.h>
#include <NyLang/Core/Host/Registry.h>
#include <NyLang/Native/Extensions/BuiltinExtension.h>
#include <NyLang/Native/Extensions/UtilExtension.h>
#include <NyLang/Native/Symbols/SymbolType.h>
#include <NyLang/Native/Symbols/SymbolValue.h>
#include <NyLang/Frontend/Node/Node.h>
#include <NyLang/Native/Messages/NativeSemanticMessages.h>
#include <NyLang/Native/Symbols/Values/SymbolValueString.h>
#include <NyLang/Native/Symbols/Values/SymbolValueInteger.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeArray.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeFP.h>
#include <NyLang/Native/Symbols/Types/SymbolTypeDelegate.h>
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

DJINN_NS3(Data, NyShader);

MaterialInbuilts::MaterialInbuilts() : IUnknown() {

}

ClassID MaterialInbuilts::GetClassID() {
    return kCID;
}

namespace Djinn::Data::NyShader {
    /// Compiler messages.
    // LANG_SIMPLEMESSAGE(SemanticMessageExpectedTexture, NyLang::MessageSeverity::eError, stream << "Expected texture value");
}

COM::Result NyShader::InstallMaterialOperations(NyLang::Registry* registry) {
    using namespace NyLang;

    // Get builtin extension
    auto builtinExtension = registry->GetClass<BuiltinExtension>();
    auto utilExtension = registry->GetClass<UtilExtension>();
    auto compiler = registry->GetInterface<ICompiler>();
    auto tokenizer = compiler->GetTokenizer();
    auto parser = compiler->GetParser();
    auto semantic = compiler->GetSemantic();

    // Install inbuilts
    auto inbuilts = registry->context.New<MaterialInbuilts>();
    registry->AddClass(inbuilts);

    // Type builtin
    builtinExtension->AddBuiltin("__material_type", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<NyLang::Symbol*> &args) -> Symbol* {
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context.New<SymbolValueString>(registry->context, NyLang::String(inbuilts->typePath.AsCharArray()))
                )
        });
    });

    // Layout builtin
    builtinExtension->AddBuiltin("__material_layout", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<NyLang::Symbol*> &args) -> Symbol* {
        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                registry->context.New<SymbolValueInfo>(
                        registry->context.New<SymbolValueString>(registry->context, NyLang::String(inbuilts->layoutPath.AsCharArray()))
                )
        });
    });

    // __returnof builtin
    builtinExtension->AddBuiltin("__returnof", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<NyLang::Symbol*> &args) -> Symbol* {
        auto arg = args[0];

        auto type = utilExtension->ResolveTypeOrValueTypeInfo(context, node, scopes, {arg});

        auto func = NyLang::OuterCast<NyLang::SymbolTypeDelegate>(type.component->type);

        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                func->returnType
        });
    });

    // __argof builtin.
    builtinExtension->AddBuiltin("__argof", [=](SemanticContext *context, Node *node, const SemanticScopeStack &scopes, const NyLang::Vector<NyLang::Symbol*> &args) -> Symbol* {
        auto arg = args[0];

        auto type = utilExtension->ResolveTypeOrValueTypeInfo(context, node, scopes, {arg});

        auto func = NyLang::OuterCast<NyLang::SymbolTypeDelegate>(type.component->type);

        return registry->context.New<Symbol>(node->GetAnchor(), NyLang::Vector<SymbolComponent *>{
                func->parameterTypes.front()
        });
    });

    // OK
    return COM::kOK;
}
