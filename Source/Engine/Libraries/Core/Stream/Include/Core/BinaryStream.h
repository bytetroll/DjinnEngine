#pragma once

#include <Core/String.h>
#include "IStream.h"

namespace Djinn {
    namespace Core {
        class BinaryStream {
        public:
            /// Construct from stream
            /// \param stream
            BinaryStream(IStream *stream) : stream(stream) {

            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(bool value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(char value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(Int8 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(Int16 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(Int32 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(Int64 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(UInt8 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(UInt16 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(UInt32 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(UInt64 value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \param value
            /// \return
            DJINN_INLINE BinaryStream &operator<<(ULong value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(float value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(double value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic write
            /// \return
            DJINN_INLINE BinaryStream &operator<<(wchar_t value) {
                stream->Write(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(bool &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(char &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(Int8 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(Int16 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(Int32 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(Int64 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(UInt8 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(UInt16 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(UInt32 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(UInt64 &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(ULong &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(float &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(double &value) {
                stream->Read(value);
                return *this;
            }

            /// Arithmetic read
            /// \return
            DJINN_INLINE BinaryStream &operator>>(wchar_t &value) {
                stream->Read(value);
                return *this;
            }

            /// Get the underlying stream
            /// \return
            IStream *GetStream() const {
                return stream;
            }

        private:
            IStream *stream;
        };
    }
}