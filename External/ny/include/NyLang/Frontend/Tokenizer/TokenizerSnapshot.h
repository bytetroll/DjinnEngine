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

#include <cstddef>
#include <NyLang/Core/Shared.h>
#include <NyLang/Frontend/Token/Token.h>

namespace NyLang {
    struct TokenizerContext;

    /// Tokenizer interface
    class TokenizerSnapshot {
    public:
        /// Construct
        /// \param ptr
        TokenizerSnapshot(TokenizerContext* context, const char* ptr);

        /// Offset current location by value
        /// \param value
        inline bool Offset(unsigned value = 1) {
            for (unsigned i = 0; i < value; i++) {
                offset.Start++;
                if (ptr[offset.Start - 1] == '\n') {
                    offset.Line++;
                    offset.Char = 1;
                } else {
                    offset.Char++;
                }
            }
            return !IsEOF();
        }

        /// Read character
        /// \return
        inline char Read() {
            if (IsEOF()) {
                return 0;
            }
            auto index = offset.Start;
            Offset(1);
            return ptr[index];
        }

        /// Peek character
        /// \param peekOffset - 0 indicates the next character
        /// \return
        inline char Peek(std::size_t peekOffset = 0) {
            if (IsEOF(peekOffset)) {
                return 0;
            }
            return ptr[offset.Start + peekOffset];
        }

        /// Is the current offset + peekOffset beyond this snapshot?
        /// \param peekOffset
        /// \return
        inline bool IsEOF(std::size_t peekOffset = 0) {
            return (offset.Start + peekOffset) >= size;
        }

        /// Get location
        /// \return
        inline TokenLoc& GetLocation() {
            return offset;
        }

        // Context
        TokenizerContext* context = nullptr;

    private:

        // Offset
        TokenLoc offset;

        // Size
        std::size_t size = 0;

        // Data
        const char* ptr = nullptr;
    };
}