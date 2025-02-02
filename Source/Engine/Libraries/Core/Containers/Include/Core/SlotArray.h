#pragma once

#include <vector>
#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Template/Lambda.h>
#include <algorithm>
#include <Core/Memory.h>
#include <Core/Assert.h>
#include <Core/Size.h>

namespace Djinn {
    namespace Core {
        /// Slot Array
        /// Prevents reallocations and offers near zero overhead removal of elements
        template<class T>
        class SlotArray {
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
                    //while (!(*++handle));
                    ++handle;
                    return *this;
                }

                IteratorBase &operator--() {
                    //while (!(*--handle));
                    --handle;
                    return *this;
                }
            };

            struct Iterator : IteratorBase<typename std::vector<T>::iterator> {
                Iterator(const typename std::vector<T>::iterator &it) : IteratorBase<typename std::vector<T>::iterator>(it) {

                }

                T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::iterator>::handle);
                }

                T *operator->() {
                    return &(*IteratorBase<typename std::vector<T>::iterator>::handle);
                }
            };

            struct ReverseIterator : IteratorBase<typename std::vector<T>::reverse_iterator> {
                ReverseIterator(const typename std::vector<T>::reverse_iterator &it) : IteratorBase<typename std::vector<T>::reverse_iterator>(it) {

                }

                T &operator*() const {
                    return *IteratorBase<typename std::vector<T>::reverse_iterator>::handle;
                }

                T *operator->() {
                    return &(*IteratorBase<typename std::vector<T>::reverse_iterator>::handle);
                }
            };

            struct ConstIterator : IteratorBase<typename std::vector<T>::const_iterator> {
                ConstIterator(const typename std::vector<T>::const_iterator &it) : IteratorBase<typename std::vector<T>::const_iterator>(it) {

                }

                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_iterator>::handle);
                }
            };

            struct ReverseConstIterator : IteratorBase<typename std::vector<T>::const_reverse_iterator> {
                ReverseConstIterator(const typename std::vector<T>::const_reverse_iterator &it) : IteratorBase<typename std::vector<T>::const_reverse_iterator>(it) {

                }

                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_reverse_iterator>::handle);
                }
            };

            SlotArray() {

            }

            /// Initialize from initializer list
            /// \param lst
            SlotArray(const std::initializer_list<T> &lst) : handle(lst) {

            }

            /// Initialize from array
            /// \tparam N
            /// \param array
            template<USize N>
            SlotArray(T(&&array)[N]) : handle(std::make_move_iterator(std::begin(array)), std::make_move_iterator(std::end(array))) {
                // ...
            }

            /// Resize this array
            /// \param size
            DJINN_INLINE void Resize(USize size) {
                handle.resize(size);
            }

            /// Reserve this array, does not move insertion index
            /// \param size
            DJINN_INLINE void Reserve(USize size) {
                handle.reserve(size);
            }

            /// Resize this array if needed
            /// \param size
            DJINN_INLINE void ResizePassive(USize size) {
                if (size > handle.size()) {
                    handle.resize(size);
                }
            }

            /// Add value to the back of this array
            /// \param value
            DJINN_INLINE void Add(T &&value) {
                Add(value);
            }

            /// Add value to the back of this array
            DJINN_INLINE void Add(const T &value) {
                if (nullHead >= handle.size()) {
                    handle.push_back(value);
                } else {
                    handle[nullHead] = value;
                }
                nullHead++;
            }

            /// Add value array to the back of this array
            DJINN_INLINE void AddArray(USize count, const T *values) {
                if (nullHead + count - 1 >= handle.size()) {
                    handle.resize(nullHead + count);
                }
                for (USize i = 0; i < count; i++) {
                    handle[nullHead + i] = values[i];
                }
                nullHead += count;
            }

            /// Pop the last element of this array
            /// \return
            DJINN_INLINE T PopEnd() {
                DJINN_ASSERT(Size() > 0, "Popping empty array");
                T value = Last();
                Remove(Size() - 1);
                return value;
            }

            /// Remove by index
            /// \param index
            /// \return
            DJINN_INLINE bool Remove(USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                std::swap(handle[index], handle[nullHead - 1]);
                nullHead--;
                return true;
            }

            /// Remove by value
            /// \param value
            /// \return
            DJINN_INLINE bool RemoveValue(const T &value) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (handle[i] == value) {
                        Remove(static_cast<USize>(i));
                        return true;
                    }
                }
                return false;
            }

            /// Check if this array contains value
            /// \param value
            /// \return
            DJINN_INLINE bool Contains(const T &value) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (handle[i] == value) {
                        return true;
                    }
                }
                return false;
            }

            /// Check if any value matches predecate
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        return true;
                    }
                }
                return false;
            }

            /// Check if any value matches predecate
            /// \param functor
            /// \param outIndex
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor, USize &outIndex) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        outIndex = i;
                        return true;
                    }
                }
                return false;
            }

            /// Check if all values matches predecate
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool All(FUNCTOR functor) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (!functor(handle[i])) {
                        return false;
                    }
                }
                return true;
            }

            /// Remove all values matching predecate
            /// \tparam FUNCTOR
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE void RemoveAll(FUNCTOR functor) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        Remove(static_cast<USize>(i));
                    }
                }
            }

            /// Remove first element matching predecate
            /// \tparam FUNCTOR
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE bool RemoveOnce(FUNCTOR functor) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        Remove(static_cast<USize>(i));
                        return true;
                    }
                }
                return false;
            }

            /// Get the size of this array
            /// \return
            DJINN_INLINE USize Size() const {
                return nullHead;
            }

            /// Clear this array
            DJINN_INLINE void Clear() {
                handle.clear();
                nullHead = 0;
            }

            /// Get element
            /// \param index
            /// \return
            DJINN_INLINE T &Get(USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get element
            /// \param index
            /// \return
            DJINN_INLINE T &operator[](USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get element
            /// \param index
            /// \return
            DJINN_INLINE const T &operator[](USize index) const {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get first element
            /// \return
            DJINN_INLINE T &First() {
                DJINN_ASSERT(Size() > 0, "Getting first element of empty array");
                return handle.front();
            }

            /// Get last element
            /// \return
            DJINN_INLINE T &Last() {
                DJINN_ASSERT(Size() > 0, "Getting last element of empty array");
                return handle[nullHead - 1];
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
                return Iterator{handle.end()};
            }

            // Get end iterator
            DJINN_INLINE ReverseIterator REnd() {
                return ReverseIterator{handle.rbegin() + nullHead};
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
                return ConstIterator{handle.begin() + nullHead};
            }

            // Get end iterator
            DJINN_INLINE ReverseConstIterator REnd() const {
                return ReverseConstIterator{handle.rbegin() + nullHead};
            }

            /// Get data pointer
            /// \return
            DJINN_INLINE T *Ptr() {
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

            USize nullHead = 0;

            // Backend handler
            std::vector<T> handle;
        };
    }
}