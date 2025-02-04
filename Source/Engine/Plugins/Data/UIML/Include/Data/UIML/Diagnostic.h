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
 on 5/6/2018.
//
#pragma once

#include <Core/String.h>
#include <Core/IStream.h>
#include <Core/StringStream.h>
#include <Core/Array.h>
#include "Token.h"

namespace Djinn::Data::UIML {
    enum class Severity {
        eInfo,
        eWarning,
        eError
    };

    class Diagnostic {
    public:
        Diagnostic(const char *source) : source(source) {
            sourceLength = std::strlen(source);
        }

        void Add(Severity severity, const Span &span, const Core::String &content, const Core::String &hint = "") {
            Message msg;
            msg.severity = severity;
            msg.span = span;
            msg.content = content;
            msg.hint = hint;
            messages.Add(msg);
        }

        void Print(Core::IStream *stream) {
            Core::StringStream ss(stream);

            for (auto &msg : messages) {
                switch (msg.severity) {
                    case Severity::eInfo:
                        ss.Write("Info    ");
                        break;
                    case Severity::eWarning:
                        ss.Write("Warning ");
                        break;
                    case Severity::eError:
                        ss.Write("Error   ");
                        break;
                }

                ss.Write(Core::String("{") + msg.span.begin.line + " : " + msg.span.begin.character + "} " + msg.content + "\n");

                const char *offset = source + msg.span.begin.index;
                std::size_t len = std::min(124u, msg.span.end.index - msg.span.begin.index);

                for (std::size_t i = 0; i < len; i++) {
                    if (offset[i] == '\n') {
                        len = i;
                        break;
                    }
                }

                std::size_t minOffset = 0;
                for (std::size_t i = 0; i < 35; i++) {
                    if ((offset - i - 1 < source) || (offset[-1 - i] == '\n')) {
                        break;
                    }
                    minOffset++;
                }

                while (isspace(*(offset - minOffset)) && minOffset != 0) {
                    minOffset--;
                }

                std::size_t maxOffset = 0;
                for (std::size_t i = 0; i < 35; i++) {
                    if ((offset - i + 1 > source + sourceLength) || (offset[len + i] == '\n')) {
                        break;
                    }
                    maxOffset++;
                }

                ss.Write("\t");
                if (minOffset != 0) {
                    stream->Write(offset - minOffset, minOffset);
                }
                if (len != 0) {
                    stream->Write(offset, len);
                }
                if (maxOffset != 0) {
                    stream->Write(offset + len , maxOffset);
                }

                if (len != (msg.span.end.index - msg.span.begin.index)) {
                    ss.Write("(cut off)\n");
                } else {
                    ss.Write("\n");
                }

                ss.Write("\t");
                for (std::size_t i = 0; i < minOffset; i++) {
                    ss.Write(" ");
                }
                ss.Write("^");
                for (std::int64_t i = 0; i < static_cast<std::int64_t>(len - 1); i++) {
                    ss.Write("~");
                }
                ss.Write("\n");

                if (msg.hint.Length() > 0) {
                    ss.Write("\t(" + msg.hint + ")\n");
                }
            }
        }

        bool HasMessages() const {
            return messages.Size() > 0;
        }

    private:
        struct Message {
            Severity severity;
            Span span;
            Core::String content;
            Core::String hint;
        };

        const char *source;
        USize sourceLength;
        Core::Array<Message> messages;
    };
}