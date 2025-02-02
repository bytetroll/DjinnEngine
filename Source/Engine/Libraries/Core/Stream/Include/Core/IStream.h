#pragma once

#include <Core/Size.h>

namespace Djinn {
    namespace Core {
        class IStream {
        public:
            virtual ~IStream() {

            }

            /// Is end of stream
            /// \return
            virtual bool IsEOS() const = 0;

            /// Is this stream good / valid?
            /// \return
            virtual bool IsGood() = 0;

            /// Close this stream
            virtual void Close() = 0;

            /// Flush any intermediate data
            virtual void Flush() = 0;

            /// Peek next integer
            /// \return
            virtual Int32 Peek() = 0;

            /// Read integer from this stream
            /// \return
            virtual Int32 Read() = 0;

            /// Read buffered from this stream
            /// \param buffer
            /// \param size
            /// \return
            virtual Int64 Read(void *buffer, USize size) = 0;

            /// Write buffered to this stream
            /// \param buffer
            /// \param size
            /// \return
            virtual Int64 Write(const void *buffer, USize size) = 0;

            /// Get the current position
            /// \return
            virtual USize GetPosition() const = 0;

            /// Get the total size
            /// \return
            virtual USize GetSize() const = 0;

            /// Set the current position
            /// \param n
            virtual void SetPosition(USize n) = 0;

            // Read helper
            template<typename T>
            Int64 Read(T& out) {
                return Read(&out, sizeof(T));
            }

            // Write helper
            template<typename T>
            Int64 Write(const T& value) {
                return Write(&value, sizeof(T));
            }
        };
    }
}