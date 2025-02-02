#pragma once

#include <cstddef>

namespace Djinn {
    namespace Core {
        template<class T>
        struct Optional {
            /// Initialize empty
            Optional(std::nullptr_t = nullptr) {

            }

            /// Initialize with value
            /// \param value
            Optional(const T &value) : value(value), valid(true) {

            }

            /// Initialize with value
            /// \param value
            Optional(T &&value) : value(value), valid(true) {

            }

            /// Assign
            /// \param other
            /// \return
            Optional& operator=(const Optional& other) {
                valid = other.valid;
                value = other.value;
                return *this;
            }

            /// Implicit check
            /// \return
            operator bool() {
                return valid;
            }

            /// Is this optional valid?
            /// \return
            bool IsValid() const {
                return valid;
            }

            /// Get value
            /// \return
            const T& Get() const {
                return value;
            }

            /// Get value
            /// \return
            T& Get() {
                return value;
            }

            /// Access value
            /// \return
            T *operator->() {
                return &value;
            }

            /// Access value
            /// \return
            const T *operator->() const {
                return &value;
            }

        private:
            T value = T();
            bool valid = false;
        };
    }
}