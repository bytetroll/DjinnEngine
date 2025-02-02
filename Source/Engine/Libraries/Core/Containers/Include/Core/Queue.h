#pragma once

#include <deque>
#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Size.h>

namespace Djinn {
    namespace Core {
        /// First in first out queue
        /// \tparam T
        template<class T>
        class Queue {
        public:
            /// Clear queue
            /// \param eraseMemory
            DJINN_INLINE void Clear(bool eraseMemory) {
                handle.clear();
                if (eraseMemory) {
                    handle.shrink_to_fit();
                }
            }

            /// Add element to this queue
            /// \param value
            DJINN_INLINE void Add(T &&value) {
                handle.push_back(value);
            }

            /// Add element to this queue
            /// \param value
            DJINN_INLINE void Add(const T &value) {
                handle.push_back(value);
            }

            /// Pop element from this queue
            /// \param out
            /// \return
            DJINN_INLINE bool Pop(T &out) {
                if (handle.empty()) {
                    return false;
                }

                // Extract
                out = handle.front();
                handle.pop_front();
                return true;
            }

            /// Peek element in this queue
            /// \param out
            /// \return
            DJINN_INLINE bool Peek(T &out) {
                if (handle.empty()) {
                    return false;
                }

                // Extract
                out = handle.front();
                return true;
            }

            /// Check if this queue is empty
            /// \return
            DJINN_INLINE bool IsEmpty() {
                return handle.empty();
            }

            /// Get the size of this queue
            /// \return
            DJINN_INLINE USize Size() {
                return handle.size();
            }

            // Backend handler
            std::deque<T> handle;
        };
    }
}