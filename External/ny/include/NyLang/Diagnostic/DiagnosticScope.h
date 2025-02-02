//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>

namespace NyLang {
    class IDiagnostic;
    class IMessage;

    class DiagnosticScope {
    public:
        DiagnosticScope(IDiagnostic* diagnostic, IMessage* msg);
        ~DiagnosticScope();

    private:
        IDiagnostic* diagnostic = nullptr;
    };
}
