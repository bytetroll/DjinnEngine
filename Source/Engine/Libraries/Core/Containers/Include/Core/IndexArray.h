#pragma once

#include <vector>
#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Template/Lambda.h>
#include <algorithm>
#include <Core/Memory.h>
#include "Array.h"

namespace Djinn {
    namespace Core {
        /// Index Array
        /// Elements are never moved during the removal or insertion of other elements
        template<class T>
        class IndexArray {
        public:
            // Iterator
            template<typename ITT>
            struct IteratorBase {
                ITT handle;
                ITT begin;
                ITT end;

                IteratorBase(const ITT& t, const ITT& begin, const ITT& end) : handle(t), begin(begin), end(end) {
                    while (handle != end && !*handle) {
                        ++handle;
                    }
                }

                bool operator==(const IteratorBase &other) const {
                    return handle == other.handle;
                }

                bool operator!=(const IteratorBase &other) const {
                    return handle != other.handle;
                }

                IteratorBase &operator++() {
                    while (++handle != end && !*handle) ;
                    return *this;
                }

                IteratorBase &operator--() {
                    while (--handle != end && !*handle) ;
                    --handle;
                    return *this;
                }
            };
            struct Iterator : IteratorBase<typename std::vector<T>::iterator> {
                Iterator(const typename std::vector<T>::iterator& it, const typename std::vector<T>::iterator& begin, const typename std::vector<T>::iterator& end)
                        : IteratorBase<typename std::vector<T>::iterator>(it, begin, end) {
                }

                T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::iterator>::handle);
                }

                T *operator->() {
                    return &(*IteratorBase<typename std::vector<T>::iterator>::handle);
                }
            };
            struct ReverseIterator : IteratorBase<typename std::vector<T>::reverse_iterator> {
                ReverseIterator(const typename std::vector<T>::reverse_iterator& it, const typename std::vector<T>::reverse_iterator& begin, const typename std::vector<T>::reverse_iterator& end)
                        : IteratorBase<typename std::vector<T>::reverse_iterator>(it, begin, end) {

                }
                T &operator*() const {
                    return *IteratorBase<typename std::vector<T>::reverse_iterator>::handle;
                }

                T *operator->() {
                    return &(*IteratorBase<typename std::vector<T>::reverse_iterator>::handle);
                }
            };
            struct ConstIterator : IteratorBase<typename std::vector<T>::const_iterator> {
                ConstIterator(const typename std::vector<T>::const_iterator& it, const typename std::vector<T>::const_iterator& begin, const typename std::vector<T>::const_iterator& end)
                        : IteratorBase<typename std::vector<T>::const_iterator>(it, begin, end) {

                }
                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_iterator>::handle);
                }
            };
            struct ReverseConstIterator : IteratorBase<typename std::vector<T>::const_reverse_iterator> {
                ReverseConstIterator(const typename std::vector<T>::const_reverse_iterator& it, const typename std::vector<T>::const_reverse_iterator& begin, const typename std::vector<T>::const_reverse_iterator& end)
                        : IteratorBase<typename std::vector<T>::const_reverse_iterator>(it, begin, end) {

                }
                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_reverse_iterator>::handle);
                }
            };

            IndexArray() {

            }

            /// Initialize from initializer list
            /// \param lst
            IndexArray(const std::initializer_list<T> &lst) : handle(lst) {

            }

            /// Initialize from array
            /// \tparam N
            /// \param array
            template<USize N>
            IndexArray(T(&&array)[N]) : handle(std::make_move_iterator(std::begin(array)), std::make_move_iterator(std::end(array))) {
                // ...
            }

            /// Resize this array
            /// \param size
            DJINN_INLINE void Resize(USize size) {
                handle.resize(size);
                totalSize = size;
            }

            /// Reserve this array, insertion index does not change
            /// \param size
            DJINN_INLINE void Reserve(USize size) {
                handle.reserve(size);
                totalSize = size;
            }

            /// Resize this array if needed
            /// \param size
            DJINN_INLINE void ResizePassive(USize size) {
                if (size > handle.size()) {
                    handle.resize(size);
                    totalSize = size;
                }
            }

            /// Add to the back of this array
            /// \param value
            /// \return
            DJINN_INLINE USize Add(T &&value) {
                return Add(value);
            }

            /// Add to the back of this array
            /// \param value
            /// \return
            DJINN_INLINE USize Add(const T &value) {
                size++;
                if (nullIndices.Size() > 0) {
                    USize index = nullIndices.PopEnd();
                    handle[index] = value;
                    return index;
                }
                totalSize++;
                handle.push_back(value);
                return handle.size() - 1;
            }

            /// Remove by index
            /// \param index
            /// \return
            DJINN_INLINE bool Remove(USize index) {
                handle[index] = nullptr;
                size--;
                nullIndices.Add(index);
                return false;
            }

            /// Remove a single value
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

            /// Check if all elements matches predecate
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

            /// Remove all values matches predecate
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE void RemoveAll(FUNCTOR functor) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        Remove(static_cast<USize>(i));
                    }
                }
            }

            /// Get the size of this array
            /// \return
            DJINN_INLINE USize Size() const {
                return size;
            }

            /// Get the total size of this array, including empty slots
            /// \return
            DJINN_INLINE USize ElementSize() const {
                return totalSize;
            }

            /// Clear this array
            DJINN_INLINE void Clear() {
                handle.clear();
                nullIndices.Clear();
            }

            /// Get element at index
            /// \param index
            /// \return
            DJINN_INLINE T &Get(USize index) {
                DJINN_ASSERT(index < handle.size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get element at index
            /// \param index
            /// \return
            DJINN_INLINE T &operator[](USize index) {
                DJINN_ASSERT(index < handle.size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get element at index
            /// \param index
            /// \return
            DJINN_INLINE const T &operator[](USize index) const {
                DJINN_ASSERT(index < handle.size(), "Array index out of bounds");
                return handle[index];
            }
            /// Get element at index
            /// \param index
            /// \return
            DJINN_INLINE T &At(USize index) {
                DJINN_ASSERT(index < handle.size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get first element
            /// \return
            DJINN_INLINE T &First() {
                return handle.front();
            }

            /// Get loast element
            /// \return
            DJINN_INLINE T &Last() {
                return handle.back();
            }

            // Get start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator {handle.begin(), handle.begin(), handle.end()};
            }

            // Get reverse start iterator
            DJINN_INLINE ReverseIterator RStart() {
                return ReverseIterator {handle.rbegin(), handle.rbegin(), handle.rend()};
            }

            // Get end iterator
            DJINN_INLINE Iterator End() {
                return Iterator {handle.end(), handle.begin(), handle.end()};
            }

            // Get end iterator
            DJINN_INLINE ReverseIterator REnd() {
                return ReverseIterator {handle.rend(), handle.rbegin(), handle.rend()};
            }

            // Get start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator {handle.begin(), handle.begin(), handle.end()};
            }

            // Get start iterator
            DJINN_INLINE ReverseConstIterator RStart() const {
                return ReverseConstIterator {handle.rbegin(), handle.rbegin(), handle.rend()};
            }

            // Get end iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator {handle.end(), handle.begin(), handle.end()};
            }

            // Get end iterator
            DJINN_INLINE ReverseConstIterator REnd() const {
                return ReverseConstIterator {handle.rend(), handle.rbegin(), handle.rend()};
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

            USize totalSize = 0;
            USize size = 0;

            // Backend handler
            std::vector<T> handle;

            Array<USize> nullIndices;
        };
    }
}