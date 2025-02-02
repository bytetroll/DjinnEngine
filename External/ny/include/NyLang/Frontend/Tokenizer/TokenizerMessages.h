//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Diagnostic/Message.h>

namespace NyLang {
    /// Common messages
    LANG_SIMPLEMESSAGE(TokenizerMessageFailed, MessageSeverity::eError, stream << "Tokenizer failed");
}
