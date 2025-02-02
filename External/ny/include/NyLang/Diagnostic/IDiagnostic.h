//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Core/Shared.h>
#include <list>

namespace NyLang {
    class IMessage;
    class DiagnosticBucket;
    class DiagnosticMessage;

    class IDiagnostic : public IUnknown {
    public:
        LANG_INTERFACEID("Lang.IDiagnosticStack");

        /// Push diagnostic scope
        /// \param scope
        virtual void Push(IMessage* scope) = 0;

        /// Push diagnostic bucket
        /// \param scope
        virtual void PushBucket(DiagnosticBucket* scope) = 0;

        /// Pop diagnostic scope
        /// \param scope
        virtual void Pop() = 0;

        /// Pop diagnostic bucket
        /// \param scope
        virtual void PopBucket() = 0;

        /// Queue a message
        /// \param message
        /// \param isRequired
        virtual void Queue(IMessage* message) = 0;

        /// Queue a diagnostic bucket onto the current bucket
        /// \param message
        /// \param isRequired
        virtual void QueueBucket(DiagnosticBucket* bucket) = 0;

        /// Pretty print all queued messages
        /// \param stream
        virtual void PrettyPrint(PrettyStream& stream) = 0;

        /// Clear all diagnostic messages
        virtual void Clear() = 0;
    };
}