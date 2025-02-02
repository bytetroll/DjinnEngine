//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Frontend/Token/Token.h>
#include "DiagnosticChain.h"

namespace NyLang {
    enum class MessageSeverity {
        eInfo,
        eWarning,
        eError,
        eInternal
    };

    class IMessage {
    public:
        /// Print this message into given stream
        /// \param stream
        virtual void PrettyPrint(PrettyStream& stream, DiagnosticChain& chain) = 0;

        /// Compose this message
        /// \param stream
        virtual void Compose(PrettyStream& stream, DiagnosticChain& chain);

        /// Get location of this message
        /// \return
        virtual TokenLoc GetLocation() = 0;

        /// Get severity of this message
        /// \return
        virtual MessageSeverity GetSeverity() = 0;
    };
}