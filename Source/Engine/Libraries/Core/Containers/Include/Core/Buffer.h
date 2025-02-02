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
 on 4/10/2018.
//
#pragma once

#include <Core/Memory.h>
#include "Array.h"

namespace Djinn::Core {
    /// A collection of elements optimized for speed
    /// \tparam T
    template<typename T>
    class Buffer {
    public:
        /// Convert to array view
        /// \return
        operator ArrayView<T>() {
            return ArrayView(Ptr(), Size());
        }

        /// Clear this buffer, does not deconstruct
        void Clear() {
            size = 0;
        }

        /// Add a new value
        /// \param value
        void Add(const T& value) {
            if (size >= array.Size()) {
                array.Add(value);
            } else {
                array[size] = value;
            }
            size++;
        }

        /// Add a new value
        /// \param value
        void Add(const T&& value) {
            Add(value);
        }

        /// Add an array to this buffer
        /// \param buffer
        void Add(const T* ptr, USize count) {
            Resize(size + count);
            Memory::Copy(array.Ptr() - count, ptr, count * sizeof(T));
        }

        /// Add another buffer to this buffer
        /// \param buffer
        void Add(const Buffer& buffer) {
            if ((size + buffer.Size() - 1) >= array.Size()) {
                array.Resize(size + buffer.Size());
            }
            for (USize i = 0; i < buffer.Size(); i++) {
                array.handle[size + i] = buffer.array.handle[i];
            }
            size += buffer.Size();
        }

        /// Add multiple values
        template<typename A0, typename A1, typename... AX>
        DJINN_INLINE void Add(const A0& a0, const A1& a1, const AX&... ax) {
            Add(a0);
            Add(a1, ax...);
        }

        /// Get size of this buffer
        /// \return
        USize Size() const {
            return size;
        }

        /// Get the byte size of this buffer
        /// \return
        USize ByteSize() {
            return array.ByteSize();
        }

        /// Does this buffer contain a value?
        /// \param value
        /// \return
        DJINN_INLINE bool Contains(const T &value) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (array[i] == value) {
                    return true;
                }
            }
            return false;
        }

        /// Does this buffer contain a value?
        /// \param value
        /// \param index - Index of the found value
        /// \return
        DJINN_INLINE bool Contains(const T &value, USize& index) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (array[i] == value) {
                    index = i;
                    return true;
                }
            }
            return false;
        }

        /// Checks if any element matches predecate
        /// \param functor
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(array[i])) {
                    return true;
                }
            }
            return false;
        }

        /// Checks if any element matches predecate
        /// \param functor
        /// \param index - Index of element
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor, USize& index) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(array[i])) {
                    index = i;
                    return true;
                }
            }
            return false;
        }

        /// Iterate all elements by predecate
        /// \param functor
        template<typename FUNCTOR>
        DJINN_INLINE void Foreach(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                functor(array[i]);
            }
        }

        /// Checks if all values matches given predecate
        /// \tparam FUNCTOR
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool All(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (!functor(array[i])) {
                    return false;
                }
            }
            return true;
        }

        /// Get first element matching a predecate or default
        /// \param functor
        /// \param _default
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE T FirstOrDefault(FUNCTOR functor, const T& _default = T()) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(array[i])) {
                    return array[i];
                }
            }
            return _default;
        }

        /// Sort this buffer by comparator
        /// \param comparator
        template<typename FUNCTOR>
        DJINN_INLINE void Sort(FUNCTOR comparator) {
            std::sort(array.handle.begin(), array.handle.begin() + size, [&](auto& a, auto& b) {
                return comparator(a, b);
            });
        }

        /// Pop an element from this buffer
        /// \return
        T PopEnd() {
            DJINN_ASSERT(size > 0, "Cannot pop empty buffer");
            return array[--size];
        }

        /// Pop an element from this buffer
        /// \return
        bool PopEnd(T& value) {
            if (size == 0) {
                return false;
            }

            value = array[--size];
            return true;
        }

        /// Get data pointer
        /// \return
        T* Ptr() {
            return array.Ptr();
        }

        /// Resize this array, does not deconstruct or construct values
        /// \param size
        void Resize(USize size) {
            if (size > array.Size()) {
                array.Resize(size);
            }
            this->size = size;
        }

        /// Get first value
        /// \return
        T& First() {
            return array.First();
        }

        /// Get last value
        /// \return
        T& Last() {
            return array[size - 1];
        }

        /// Get element at index
        /// \param i
        /// \return
        T& operator[](USize i) {
            DJINN_ASSERT(i < Size(), "Buffer index out of bounds");
            return array[i];
        }

        /// Get element at index
        /// \param i
        /// \return
        const T& operator[](USize i) const {
            DJINN_ASSERT(i < Size(), "Buffer index out of bounds");
            return array[i];
        }

        // Get start iterator
        DJINN_INLINE typename Core::Array<T>::Iterator Start() {
            return array.Start();
        }

        // Get reverse start iterator
        DJINN_INLINE typename Core::Array<T>::ReverseIterator RStart() {
            return array.RStart();
        }

        // Get end iterator
        DJINN_INLINE typename Core::Array<T>::Iterator End() {
            return array.Start() + size;
        }

        // Get end iterator
        DJINN_INLINE typename Core::Array<T>::ReverseIterator REnd() {
            return array.RStart() + size;
        }

        // Get start iterator
        DJINN_INLINE typename Core::Array<T>::ConstIterator Start() const {
            return array.Start();
        }

        // Get start iterator
        DJINN_INLINE typename Core::Array<T>::ReverseConstIterator RStart() const {
            return array.RStart();
        }

        // Get end iterator
        DJINN_INLINE typename Core::Array<T>::ConstIterator End() const {
            return array.RStart() + size;
        }

        // Get end iterator
        DJINN_INLINE typename Core::Array<T>::ReverseConstIterator REnd() const {
            return array.RStart() + size;
        }

        /// Range expression
        DJINN_INLINE typename Core::Array<T>::Iterator begin() {
            return Start();
        }

        /// Range expression
        DJINN_INLINE typename Core::Array<T>::ConstIterator begin() const {
            return Start();
        }

        /// Range expression
        DJINN_INLINE typename Core::Array<T>::Iterator end() {
            return End();
        }

        /// Range expression
        DJINN_INLINE typename Core::Array<T>::ConstIterator end() const {
            return End();
        }

    private:
        USize size = 0;
        Core::Array<T> array;
    };
}