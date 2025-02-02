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
 on 10/29/2017.
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