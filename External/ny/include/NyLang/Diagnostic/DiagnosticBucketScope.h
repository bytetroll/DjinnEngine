//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>

namespace NyLang {
    class IDiagnostic;
    class DiagnosticBucket;

    class DiagnosticBucketScope {
    public:
        DiagnosticBucketScope(IDiagnostic* diagnostic, DiagnosticBucket* bucket);
        ~DiagnosticBucketScope();

    private:
        IDiagnostic* diagnostic = nullptr;
    };
}
