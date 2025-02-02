//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 10/29/2017.
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