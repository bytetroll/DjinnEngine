//
// Created by Dev on 5/28/2018.
//
#pragma once

#include <clang-c/Index.h>
#include <Core/String.h>
#include <Core/Array.h>

namespace Djinn::DHG {
    struct ParserConfig {
        Core::Array<Core::String> preprocessors;
        Core::Array<Core::String> includes;
        Core::Array<Core::String> args;
    };

    class ParserBlob {
    public:
        ParserBlob(CXTranslationUnit unit);
        ~ParserBlob();

        CXTranslationUnit unit = nullptr;
    };

    class Parser {
    public:
        Parser();
        ~Parser();

        ParserBlob* Parse(const ParserConfig& config, const Core::String& name);

    private:
        Core::Array<Core::String> args;
        Core::Array<const char*> pargs;
        CXIndex index = nullptr;
    };
}