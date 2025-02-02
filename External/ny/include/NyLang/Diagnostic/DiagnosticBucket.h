//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Shared.h>
#include <list>
#include <NyLang/Core/PrettyStream.h>
#include "DiagnosticChain.h"

namespace NyLang {
    class DiagnosticMessage;

    class DiagnosticBucket {
    public:
        /// Merge with other bucket
        /// \param bucket
        void Merge(DiagnosticBucket* bucket);

        /// Pretty print messages
        /// \param stream
        void PrettyPrint(PrettyStream& stream, DiagnosticChain& chain);

        std::list<DiagnosticMessage*> messages;
    };
}
