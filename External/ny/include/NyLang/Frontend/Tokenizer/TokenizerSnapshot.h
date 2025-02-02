//
// Created by Dev on 10/29/2017.
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