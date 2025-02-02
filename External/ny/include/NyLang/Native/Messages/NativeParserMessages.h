//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Diagnostic/Message.h>

namespace NyLang {
    LANG_SIMPLEMESSAGE(ParserMessageOperatorNoBinaryToken, MessageSeverity::eError, stream << "Operator expected a valid binary token to overload");
    LANG_SIMPLEMESSAGE(ParserMessageNoEndOfScope, MessageSeverity::eError, stream << "Expected end of scoped expression");
    LANG_SIMPLEMESSAGE(ParserMessageUnexpectedID, MessageSeverity::eError, stream << "Unexpected ID '" << tok.Value.Str.Value << "'");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedID, MessageSeverity::eError, stream << "Expected ID");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedDeclaration, MessageSeverity::eError, stream << "Expected declaration");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedType, MessageSeverity::eError, stream << "Expected type");
    LANG_SIMPLEMESSAGE(ParserMessageFuncSpecTakesNoArguments, MessageSeverity::eError, stream << "FunctionType specifier '" << tok.Value.Str.Value << "' doesnt take any arguments");
    LANG_SIMPLEMESSAGE(ParserMessageFuncSpecExpectsArguments, MessageSeverity::eError, stream << "FunctionType specifier '" << tok.Value.Str.Value << "' expects arguments");
    LANG_SIMPLEMESSAGE(ParserMessageExpectedBody, MessageSeverity::eError, stream << "Expected body");
    LANG_SIMPLEMESSAGE(ParserMessageTypeHasID, MessageSeverity::eError, stream << "Only one ID per type is allowed");
    LANG_SIMPLEMESSAGE(ParserMessagePostSpecifierAfter, MessageSeverity::eError, stream << "Invalid use of post specifier");
}
