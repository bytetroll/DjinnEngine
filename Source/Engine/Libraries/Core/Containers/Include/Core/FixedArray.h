#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Template/Lambda.h>
#include <Core/Template/Variadic.h>
#include <Core/Memory.h>
#include <Core/Assert.h>
#include <Core/Size.h>
#include <Core/ArrayView.h>

namespace Djinn::Core {
    /// A collection of elements with a fixed maximum size, stack allocated
    /// \tparam T - Data type
    /// \tparam SIZE - Maximum size
    template<class T, USize SIZE>
    class FixedArray {
    public:
        // Iterator
        template<typename ITT>
        struct IteratorBase {
            ITT handle;

            IteratorBase(const ITT &t) : handle(t) {

            }

            bool operator==(const IteratorBase &other) const {
                return handle == other.handle;
            }

            bool operator!=(const IteratorBase &other) const {
                return handle != other.handle;
            }

            IteratorBase &operator++() {
                handle++;
                return *this;
            }

            IteratorBase &operator--() {
                handle--;
                return *this;
            }
        };

        struct Iterator : IteratorBase<typename std::array<T, SIZE>::iterator> {
            Iterator(const typename std::array<T, SIZE>::iterator &it) : IteratorBase<typename std::array<T, SIZE>::iterator>(it) {

            }

            T &operator*() const {
                return (*IteratorBase<typename std::array<T, SIZE>::iterator>::handle);
            }

            T *operator->() {
                return &(*IteratorBase<typename std::array<T, SIZE>::iterator>::handle);
            }
        };

        struct ReverseIterator : IteratorBase<typename std::array<T, SIZE>::reverse_iterator> {
            ReverseIterator(const typename std::array<T, SIZE>::reverse_iterator &it) : IteratorBase<typename std::array<T, SIZE>::reverse_iterator>(it) {

            }

            T &operator*() const {
                return *IteratorBase<typename std::array<T, SIZE>::reverse_iterator>::handle;
            }

            T *operator->() {
                return &(*IteratorBase<typename std::array<T, SIZE>::reverse_iterator>::handle);
            }
        };

        struct ConstIterator : IteratorBase<typename std::array<T, SIZE>::const_iterator> {
            ConstIterator(const typename std::array<T, SIZE>::const_iterator &it) : IteratorBase<typename std::array<T, SIZE>::const_iterator>(it) {

            }

            const T &operator*() const {
                return (*IteratorBase<typename std::array<T, SIZE>::const_iterator>::handle);
            }
        };

        struct ReverseConstIterator : IteratorBase<typename std::array<T, SIZE>::const_reverse_iterator> {
            ReverseConstIterator(const typename std::array<T, SIZE>::const_reverse_iterator &it) : IteratorBase<typename std::array<T, SIZE>::const_reverse_iterator>(it) {

            }

            const T &operator*() const {
                return (*IteratorBase<typename std::array<T, SIZE>::const_reverse_iterator>::handle);
            }
        };

        FixedArray() {

        }

        /// Convert to array view
        /// \return
        operator ArrayView<T>() {
            return ArrayView(Ptr(), Size());
        }

        /// Add to the back of this array
        /// \param value
        DJINN_INLINE void Add(T &&value) {
            DJINN_ASSERT(size < SIZE, "Fixed array limit reached");
            handle[size++] = value;
        }

        /// Add to the back of this array
        /// \param value
        DJINN_INLINE void Add(const T &value) {
            DJINN_ASSERT(size < SIZE, "Fixed array limit reached");
            handle[size++] = value;
        }

        /// Add to the front of this array
        /// \param value
        DJINN_INLINE void AddFront(T &&value) {
            DJINN_ASSERT(size < SIZE, "Fixed array limit reached");
            Core::Memory::Copy(handle.data(), handle.data() + 1, size * sizeof(T));
            handle[0] = value;
        }

        /// Resize this array
        /// \param size
        DJINN_INLINE void Resize(USize size) {
            DJINN_ASSERT(size <= SIZE, "Size larger than fixed array limit");
            this->size = size;
        }

        /// Resize this array if needed
        /// \param size
        /// \return
        DJINN_INLINE bool ResizePassive(USize size) {
            DJINN_ASSERT(size <= SIZE, "Size larger than fixed array limit");
            if (this->size < size) {
                this->size = size;
                return true;
            }
            return false;
        }

        /// Remove a value from this array
        /// \param index
        DJINN_INLINE void Remove(USize index) {
            DJINN_ASSERT(index < Size(), "Fixed array index out of bounds");
            for (USize i = index; i < size - 1; i++) {
                handle[i] = handle[i + 1];
            }
            size--;
        }

        /// Check if this array contains a value
        /// \param value
        /// \return
        DJINN_INLINE bool Contains(const T &value) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (handle[i] == value) {
                    return true;
                }
            }
            return false;
        }

        /// Checks if any element matches given predecate
        /// \param functor
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(handle[i])) {
                    return true;
                }
            }
            return false;
        }

        /// Checks if any element matches given predecate
        /// \param functor
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(handle[i])) {
                    return true;
                }
            }
            return false;
        }

        /// Checks if any element matches given predecate
        /// \param functor
        /// \param index - Index of element
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor, USize &index) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(handle[i])) {
                    index = static_cast<USize>(i);
                    return true;
                }
            }
            return false;
        }

        /// Apply predecate to all elements
        /// \tparam FUNCTOR
        /// \param functor
        template<typename FUNCTOR>
        DJINN_INLINE void Foreach(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                functor(handle[i]);
            }
        }

        /// Checks if a predecate matches all elements
        /// \tparam FUNCTOR
        /// \param functor
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool All(FUNCTOR functor) const {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (!functor(handle[i])) {
                    return false;
                }
            }
            return true;
        }

        /// Get first element matching predecate
        /// \param functor
        /// \param _default
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE T FirstOrDefault(FUNCTOR functor, const T &_default = T()) {
            for (Int64 i = size - 1; i >= 0; i--) {
                if (functor(handle[i])) {
                    return handle[i];
                }
            }
            return _default;
        }

        /// Select values based on predecate
        /// \param functor
        /// \return
        template<typename FUNCTOR, typename R = typename LambdaFunctionTraits<FUNCTOR>::ResultType>
        DJINN_INLINE FixedArray<R, SIZE> Select(FUNCTOR functor) {
            FixedArray<R, SIZE> result;
            for (Int64 i = size - 1; i >= 0; i--) {
                result.Add(functor(handle[i]));
            }
            return result;
        }

        /// Clear this array
        DJINN_INLINE void Clear() {
            size = 0;
        }

        /// Get the size of this array
        /// \return
        DJINN_INLINE USize Size() const {
            return size;
        }

        /// Pop the last element of this array
        /// \return
        DJINN_INLINE T PopEnd() {
            DJINN_ASSERT(Size() > 0, "Attempting to pop empty fixed array");
            size--;
            return handle[size];
        }

        /// Get element by index
        /// \param index
        /// \return
        DJINN_INLINE T &Get(USize index) {
            DJINN_ASSERT(index < Size(), "Fixed array out of bounds");
            return handle[index];
        }

        /// Get element by index
        /// \param index
        /// \return
        DJINN_INLINE T &operator[](USize index) {
            DJINN_ASSERT(index < Size(), "Fixed array out of bounds");
            return handle[index];
        }

        /// Get element by index
        /// \param index
        /// \return
        DJINN_INLINE const T &operator[](USize index) const {
            DJINN_ASSERT(index < Size(), "Fixed array out of bounds");
            return handle[index];
        }

        /// Get element
        /// \param index
        /// \return
        DJINN_INLINE T &At(USize index) {
            DJINN_ASSERT(index < Size(), "Fixed array out of bounds");
            return handle[index];
        }

        /// Get first element
        /// \return
        DJINN_INLINE T &First() {
            return handle.front();
        }

        /// Get last element
        /// \return
        DJINN_INLINE T &Last() {
            return handle[size - 1];
        }

        // Get start iterator
        DJINN_INLINE Iterator Start() {
            return Iterator{handle.begin()};
        }

        // Get reverse start iterator
        DJINN_INLINE ReverseIterator RStart() {
            return ReverseIterator{handle.rbegin()};
        }

        // Get end iterator
        DJINN_INLINE Iterator End() {
            return Iterator{handle.begin() + size};
        }

        // Get end iterator
        DJINN_INLINE ReverseIterator REnd() {
            return ReverseIterator{handle.rbegin() + size};
        }

        // Get start iterator
        DJINN_INLINE ConstIterator Start() const {
            return ConstIterator{handle.begin()};
        }

        // Get start iterator
        DJINN_INLINE ReverseConstIterator RStart() const {
            return ReverseConstIterator{handle.rbegin()};
        }

        // Get end iterator
        DJINN_INLINE ConstIterator End() const {
            return ConstIterator{handle.begin() + size};
        }

        // Get end iterator
        DJINN_INLINE ReverseConstIterator REnd() const {
            return ReverseConstIterator{handle.rbegin() + size};
        }

        /// Get data pointer
        /// \return
        DJINN_INLINE T *Ptr() {
            return handle.data();
        }

        /// Get data pointer
        /// \return
        DJINN_INLINE const T *Ptr() const {
            return handle.data();
        }

        /// Range expression
        DJINN_INLINE Iterator begin() {
            return Start();
        }

        /// Range expression
        DJINN_INLINE ConstIterator begin() const {
            return Start();
        }

        /// Range expression
        DJINN_INLINE Iterator end() {
            return End();
        }

        /// Range expression
        DJINN_INLINE ConstIterator end() const {
            return End();
        }

        // Current size
        USize size = 0;

        // Backend handler
        std::array<T, SIZE> handle;
    };

    /// Common usages
    template<typename T> using FixedArray4 = FixedArray<T, 4>;
    template<typename T> using FixedArray8 = FixedArray<T, 8>;
    template<typename T> using FixedArray12 = FixedArray<T, 12>;
    template<typename T> using FixedArray16 = FixedArray<T, 16>;
    template<typename T> using FixedArray32 = FixedArray<T, 32>;
    template<typename T> using FixedArray64 = FixedArray<T, 64>;
    template<typename T> using FixedArray128 = FixedArray<T, 128>;
}