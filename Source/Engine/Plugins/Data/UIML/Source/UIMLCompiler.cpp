//
// Created by Dev on 5/5/2018.
//

#include <Data/UIMLCompiler.h>
#include <Data/UIMLConfiguration.h>
#include <Base/DefaultLogs.h>
#include <Data/BaseUIMLAsset.h>
#include <Core/StringStream.h>
#include <Data/UIML/TokenBlob.h>
#include <Data/UIML/Tokenizer.h>
#include <Data/UIML/Diagnostic.h>
#include <Core/Context.h>
#include <Core/MemoryStream.h>
#include <Core/DynamicMemoryStream.h>
#include <Data/UIML/ParserBlob.h>
#include <Data/UIML/Parser.h>
#include <Core/Timer.h>
#include <Data/UIML/SemanticBlob.h>
#include <Data/UIML/Semantic.h>

DJINN_NS2(Data);

UIMLCompiler::UIMLCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIMLCompiler(registry, outer, host) {

}

COM::Result UIMLCompiler::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result UIMLCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = reinterpret_cast<UIMLConfiguration *>(_config);

    // Diagnostic
    Core::Timer timer;
    InfoLog(this).Write("Started UIML compilation job [", config->Hash(), "]");

    // Create font asset
    BaseUIMLAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        ErrorLog(this).Write("Failed to create font asset");
        return COM::kError;
    }

    // Read stream as text
    Core::String source = Core::StringStream(stream).ReadUntil(0);

    // Prepare context
    Memory::Context context;

    // Prepare diagnostic
    UIML::Diagnostic diagnostic(source.AsCharArray());

    // Tokenize
    UIML::TokenBlob tokenBlob;
    if (UIML::Tokenizer tokenizer; !tokenizer.Initialize(&context, &diagnostic) || !tokenizer.Compile(source.AsCharArray(), &tokenBlob)) {
        Core::DynamicMemoryStream dynStream;
        diagnostic.Print(&dynStream);
        ErrorLog(this).Write("UIML compilation failed\n", Core::String(dynStream.GetBuffer(), 0, dynStream.GetSize()));
        return COM::kError;
    }

    // Parse
    UIML::ParserBlob parserBlob;
    if (UIML::Parser parser; !parser.Initialize(&context, &diagnostic) || !parser.Compile(&tokenBlob, &parserBlob)) {
        Core::DynamicMemoryStream dynStream;
        diagnostic.Print(&dynStream);
        ErrorLog(this).Write("UIML compilation failed:\n", Core::String(dynStream.GetBuffer(), 0, dynStream.GetSize()));
        return COM::kError;
    }

    // Verify semantics
    UIML::SemanticBlob semanticBlob;
    if (UIML::Semantic semantic; !semantic.Initialize(&context, &diagnostic) || !semantic.Compile(&parserBlob, &semanticBlob)) {
        Core::DynamicMemoryStream dynStream;
        diagnostic.Print(&dynStream);
        ErrorLog(this).Write("UIML compilation failed:\n", Core::String(dynStream.GetBuffer(), 0, dynStream.GetSize()));
        return COM::kError;
    }

    // Debug messages?
    if (diagnostic.HasMessages()) {
        Core::DynamicMemoryStream dynStream;
        diagnostic.Print(&dynStream);
        ErrorLog(this).Write("UIML compilation residual messages:\n", Core::String(dynStream.GetBuffer(), 0, dynStream.GetSize()));
        return COM::kError;
    }

    // Copy content
    asset->content = semanticBlob.content;

    // Diagnostic
    InfoLog(this).Write("UIML compilation finished (", timer.Delta(), "s)");

    // OK
    *out = asset;
    return COM::kOK;
}
