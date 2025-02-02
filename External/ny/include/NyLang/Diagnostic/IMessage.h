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