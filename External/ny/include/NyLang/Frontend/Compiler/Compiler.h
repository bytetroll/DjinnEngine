//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include "ICompiler.h"

namespace NyLang {
    class IDiagnostic;

    class Compiler : public ICompiler {
    public:
        LANG_CLASSID("Compiler");

        /// Construct and install instance into registry
        /// \return
        static Compiler* Install(Registry*registry);

        /// Construct
        /// \param registry
        Compiler(Registry* registry);

        /// Initialize this compiler
        /// \return
        bool Initialize();

        /// Overrides
        virtual NyLang::CompilerContext* Compile(const CompilerJobInfo &info) override;
        virtual void SetTokenizer(ITokenizer*tokenizer) override;
        virtual void SetParser(IParser*parser) override;
        virtual void SetSemantic(ISemantic*semantic) override;
        virtual NyLang::ITokenizer*GetTokenizer() override;
        virtual NyLang::IParser*GetParser() override;
        virtual NyLang::ISemantic*GetSemantic() override;
        virtual ClassID GetClassID() override;
        virtual bool Configure() override;

    private:
        ITokenizer* tokenizer = nullptr;
        IParser* parser = nullptr;
        ISemantic* semantic = nullptr;
        Registry* registry = nullptr;
        IDiagnostic* diagnostic = nullptr;
    };
}
