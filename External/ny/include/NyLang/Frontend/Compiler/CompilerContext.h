//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Shared.h>

namespace NyLang {
    class TokenizerContext;
    class ParserContext;
    struct SemanticContext;

    /// Context of a compiler
    struct CompilerContext {
        // Did it succeed?
        bool success = true;

        // Tokenizer context
        TokenizerContext* tokenizer = nullptr;

        // Parser context
        ParserContext* parser = nullptr;

        // Semantic context
        SemanticContext* semantic = nullptr;
    };
}
