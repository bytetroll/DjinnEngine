//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Frontend/Token/Token.h>
#include <NyLang/Diagnostic/Message.h>

namespace NyLang {
    /// Common messages
    LANG_SIMPLEMESSAGE(ParserMessageUnexpectedEnd, MessageSeverity::eError, stream << "Unexpected end of file");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedExpression, MessageSeverity::eError, stream << "Expected an expression");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedSegment, MessageSeverity::eError, stream << "Expected a segment");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedEOS, MessageSeverity::eError, stream << "Expected end of statement");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedDefinition, MessageSeverity::eError, stream << "Expected a definition");
    LANG_SIMPLEMESSAGE_1ARG(ParserMessageExpectedToken, MessageSeverity::eError, String, expected, stream << "Expected " << expected);
}
