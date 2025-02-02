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
 on 9/29/2018.
//
#pragma once

#include <Core/SharedPtr.h>
#include <Core/Template/Common.h>

namespace Djinn::UI {
    template<typename T>
    struct Property {
        using SharedType = Core::SharedPtr<T>;

        /// Default constructor
        /// \param args
        template<typename... A>
        Property(A &&... args) : isLinked(false) {
            new(&storage) T(args...);
        }

        /// Create from shared
        /// \param shared
        Property(const SharedType &shared) : isLinked(false) {
            new(&storage) SharedType(shared);
        }

        /// Copy constructor (Fix)
        /// \param other
        Property(Property &other) : isLinked(other.isLinked) {
            void* ptr = &other.storage;
            if (isLinked) {
                new(&storage) SharedType(*reinterpret_cast<const SharedType *>(ptr));
            } else {
                new(&storage) T(*reinterpret_cast<const T *>(ptr));
            }
        }

        /// Copy constructor
        /// \param other
        Property(const Property &other) : isLinked(other.isLinked) {
            void* ptr = &other.storage;
            if (isLinked) {
                new(&storage) SharedType(*reinterpret_cast<const SharedType *>(ptr));
            } else {
                new(&storage) T(*reinterpret_cast<const T *>(ptr));
            }
        }

        /// Move constructor
        /// \param other
        Property(Property &&other) : Property(other) {
            // ...
        }

        ~Property() {
            void* ptr = &storage;
            if (isLinked) {
                reinterpret_cast<SharedType *>(ptr)->~SharedType();
            } else {
                reinterpret_cast<T *>(ptr)->~T();
            }
        }

        /// Copy assignment operator
        /// \param other
        /// \return
        Property &operator=(const Property &other) {
            if (&other == this) {
                return *this;
            }

            void* ptr = &storage;
            if (isLinked) {
                reinterpret_cast<SharedType *>(ptr)->~SharedType();
            } else {
                reinterpret_cast<T *>(ptr)->~T();
            }

            isLinked = other.isLinked;

            const void* otherPtr = &other.storage;
            if (isLinked) {
                new(&storage) SharedType(*reinterpret_cast<const SharedType *>(otherPtr));
            } else {
                new(&storage) T(*reinterpret_cast<const T *>(otherPtr));
            }
            return *this;
        }

        /// Move assignment operator
        /// \param other
        /// \return
        Property &operator=(Property &&other) {
            *this = other;
            return *this;
        }

        /// Accessor
        /// \return
        T *operator->() {
            return Get();
        }

        /// Accessor
        /// \return
        const T *operator->() const {
            return Get();
        }

        /// Get the value
        /// \return
        T &Get() {
            void* ptr = &storage;
            if (isLinked) {
                return *reinterpret_cast<SharedType *>(ptr)->Get();
            } else {
                return *reinterpret_cast<T *>(ptr);
            }
        }

        /// Get the value
        /// \return
        const T &Get() const {
            const void* ptr = &storage;
            if (isLinked) {
                return *reinterpret_cast<const SharedType *>(ptr)->Get();
            } else {
                return *reinterpret_cast<const T *>(ptr);
            }
        }

        /// Return a new property that is linked
        /// \return
        Property<T> Link() {
            if (isLinked) {
                return *this;
            }

            void* ptr = &storage;
            return Core::MakeShared<T>(*reinterpret_cast<T *>(ptr));
        }

        /// Is this property linked?
        /// \return
        bool IsLinked() const {
            return isLinked;
        }

    private:
        /// Is this property shared?
        bool isLinked = false;

        /// Buffer
        std::aligned_storage_t<Core::MaxSize < sizeof(T), sizeof(SharedType)>, Core::MaxSize<alignof(T), alignof(SharedType)>> storage;
    };
}