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
 on 5/28/2018.
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
