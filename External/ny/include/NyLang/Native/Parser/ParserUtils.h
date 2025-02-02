//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Frontend/Parser/IParser.h>

namespace NyLang {
    namespace ParserUtils {
        /// Parse parameter list
        /// \param snapshot
        /// \param scope
        /// \param escape
        /// \param escapeStr
        /// \return
        extern ParserEvent ParseParameterList(ParserSnapshot &snapshot, ParserScopeStack scope, TokenID escape = ")"_tok, const String& escapeStr = ")");
    }
}
