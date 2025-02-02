//
// Created by Dev on 5/28/2018.
//

#include <DHG/Parser.h>
#include <Core/Common.h>
#include <iostream>

DJINN_NS2(DHG);

Parser::Parser() {
    index = clang_createIndex(0, 1);
}

Parser::~Parser() {
    clang_disposeIndex(index);
}

ParserBlob *Parser::Parse(const ParserConfig &config, const Core::String &name) {
    // Default arguments
    args.Clear();
    args.Add(
            "-x", "c++",
            "-DDJINN_DHG",
            "-ferror-limit=512",
            "-Wno-pragma-once-outside-header", "-Wno-pragma-pack"
    );
    args.Add(config.args);

    // Includes
    for (auto &include : config.includes) {
        args.Add("-I" + include);
    }

    // Preprocessors
    for (auto &preprocessor : config.preprocessors) {
        args.Add("-D" + preprocessor);
    }

    // To p-args
    pargs.Clear();
    for (auto &arg : args) {
        pargs.Add(arg.AsCharArray());
    }

    // Attempt to parse
    CXTranslationUnit unit = nullptr;
    auto error = clang_parseTranslationUnit2(
            index, name.AsCharArray(), pargs.Ptr(), static_cast<int>(pargs.Size()),
            nullptr, 0,
            CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_KeepGoing | CXTranslationUnit_DetailedPreprocessingRecord,
            &unit
    );

    // OK?
    if (error != CXError_Success) {
        int n = clang_getNumDiagnostics(unit);
        for (int i = 0; i != n; ++i) {
            std::cout << "[ DHG] Parsing error - " << clang_getCString(clang_formatDiagnostic(clang_getDiagnostic(unit, n), clang_defaultDiagnosticDisplayOptions())) << "\n";
        }
        return nullptr;
    }

    // Create blob
    ParserBlob *blob = new ParserBlob(unit);
    return blob;
}

ParserBlob::ParserBlob(CXTranslationUnit unit) : unit(unit) {

}

ParserBlob::~ParserBlob() {
    clang_disposeTranslationUnit(unit);
}
