#pragma once

#include <Core/StringView.h>
#include "IStream.h"

namespace Djinn {
    namespace Core {
        class StringStream {
        public:
            /// Construct from stream
            /// \param stream
            StringStream(IStream* stream) : stream(stream) {

            }

            /// Read
            /// \return
            char Read() {
                char ch;
                stream->Read(&ch, 1);
                if (ch == -1) {
                    return 0;
                }
                return ch;
            }

            /// Peek
            /// \return
            char Peek() {
                return 0;
            }

            /// Read until
            /// \param ch
            /// \return
            Core::String ReadUntil(char ch) {
                Core::String str;

                // Read until
                char c;
                while ((c = Read()) && (c != ch)) {
                    str.Append(c);
                }

                if (c == ch) {
                    stream->SetPosition(stream->GetPosition() - 1);
                }

                // ...
                return str;
            }

            /// Read the entire line
            /// \param ch
            /// \return
            Core::String ReadLine() {
                Core::String str;

                // Read until
                char c;
                while ((c = Read()) && (c != '\n')) {
                    if (c != '\r') {
                        str.Append(c);
                    }
                }

                // ...
                return str;
            }

            /// Read all
            /// \return
            Core::String ReadAll() {
                return ReadUntil(0);
            }

            /// Write
            /// \param str
            void Write(const Core::String& str) {
                stream->Write(str.AsCharArray(), str.Length());
            }

            /// Write
            /// \param str
            void Write(const char* str) {
                stream->Write(str, std::strlen(str));
            }

            /// Get stream
            /// \return
            IStream* GetStream() {
                return stream;
            }

            /// String write
            /// \return
            DJINN_INLINE StringStream& operator<<(const Core::String& value) {
                Write(value);
                return *this;
            }

            /// String write
            /// \return
            DJINN_INLINE StringStream& operator<<(const Core::StringView& value) {
                stream->Write(value.Ptr(), value.Length());
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(const char* value) {
                Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(bool value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(char value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(Int8 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(Int16 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(Int32 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(Int64 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(UInt8 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(UInt16 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(UInt32 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(UInt64 value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(float value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(double value) {
                Write(String(value));
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE StringStream& operator<<(wchar_t value) {
                Write(String(value));
                return *this;
            }

        private:
            IStream* stream;
        };
    }
}