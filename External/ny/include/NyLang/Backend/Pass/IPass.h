//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang::IL {
    class Program;

    class IPass : public IUnknown {
    public:
        /// Run this pass through program
        /// \param program
        /// \return
        virtual bool Pass(Program* program) = 0;
    };
}
