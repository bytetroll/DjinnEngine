//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/StdTypes.h>

namespace NyLang::IL {
    class PrettyPrintContext {
    public:
        PrettyPrintContext();

        /// Get register from pointer
        /// \param ptr
        /// \return
        int GetRegister(void* ptr);

    private:
        UnorderedMap<void*, int> registers;
    };
}
