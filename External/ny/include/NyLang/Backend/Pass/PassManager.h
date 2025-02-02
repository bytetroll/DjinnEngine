//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/StdTypes.h>

namespace NyLang::IL {
    class IPass;
    class Program;

    class PassManager {
    public:
        /// Add pass
        /// \param pass
        void AddPass(IPass* pass);

        /// Run all passes on program
        /// \param program
        void Pass(Program* program);

    private:
        PtrVector<IPass> passes;
    };
}
