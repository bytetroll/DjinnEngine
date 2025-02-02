//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Frontend/Symbol/SymbolPair.h>
#include <NyLang/Native/Symbols/Meta.h>

namespace NyLang {
    namespace SymbolUtils {
        // Switch token to symbol visibility
        extern SymbolVisibility SwitchVisibility(TokenID ID);

        // Switch token to symbol visibility with default value
        extern SymbolVisibility SwitchVisibility(TokenID ID, SymbolVisibility Visibility);

        // Switch token to type storage
        extern SymbolTypeStorage SwitchTypeStorage(TokenID ID);

        // Switch token to object storage
        extern SymbolObjectStorageSet SwitchObjectStorage(TokenID ID);

        // Switch token to function storage
        extern SymbolFunctionStorage SwitchFunctionStorage(TokenID ID, std::size_t parameters);
    }
}
