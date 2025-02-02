//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <iostream>

namespace NyLang {
    class ITokenizer;
    class IParser;
    class ISemantic;
    class CompilerContext;

    /// Compilation job description
    struct CompilerJobInfo {
        // Source code of this job
        const char *source = nullptr;

        // Output debug info?
        bool debugOutput = true;
    };

    class ICompiler : public IUnknown {
    public:
        LANG_INTERFACEID("Lang.ICompiler");

        /// Compile from a job
        /// \param info
        /// \return
        virtual CompilerContext* Compile(const CompilerJobInfo& info) = 0;

        /// Set tokenizer
        /// \param tokenizer
        virtual void SetTokenizer(ITokenizer*tokenizer) = 0;

        /// Set parser
        /// \param parser
        virtual void SetParser(IParser*parser) = 0;

        /// Set semantic
        /// \param semantic
        virtual void SetSemantic(ISemantic*semantic) = 0;

        /// Get tokenizer
        /// \param tokenizer
        virtual ITokenizer*GetTokenizer() = 0;

        /// Get parser
        /// \param parser
        virtual IParser*GetParser() = 0;

        /// Get semantic
        /// \param semantic
        virtual ISemantic*GetSemantic() = 0;

        /// Configure the compiler
        /// \return
        virtual bool Configure() = 0;
    };
}