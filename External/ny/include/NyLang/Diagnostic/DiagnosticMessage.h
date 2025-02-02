//
// Created by Dev on 11/2/2017.
//
#pragma once

#include <NyLang/Core/Shared.h>
#include <list>
#include <NyLang/Core/PrettyStream.h>
#include "DiagnosticChain.h"

namespace NyLang {
    class IMessage;

    struct DiagnosticMessage {
        void PrettyPrint(PrettyStream& stream, DiagnosticChain& chain);

        std::list<IMessage*> scopes;
        IMessage* message = nullptr;
    };
}