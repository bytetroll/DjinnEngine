#pragma once

#include <vector>
#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Template/Lambda.h>
#include <algorithm>
#include <Core/Size.h>
#include <Core/Assert.h>
#include <Core/Template/Variadic.h>
#include "ArrayView.h"

namespace Djinn {
    namespace Core {
        /// Array tags, reserved for future use
        struct TArrayNullTerminated;

        /// Array container
        /// \tparam T - Stored type
        /// \tparam Tags - Reserved for future use
        template<class T, typename... Tags>
        class Array {
        public:
            /// Is this array null terminated?
            static constexpr bool kNullTerminated = VariadicContains<TArrayNullTerminated, Tags...>::kValue;

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
            struct Iterator : IteratorBase<typename std::vector<T>::iterator> {
                Iterator(const typename std::vector<T>::iterator &it) : IteratorBase<typename std::vector<T>::iterator>(it) {

                }

                T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::iterator>::handle);
                }

                T *operator->() {
                    return &(*IteratorBase<typename std::vector<T>::iterator>::handle);
                }

                Iterator &operator+(USize n) {
                    IteratorBase<typename std::vector<T>::iterator>::handle += n;
                    return *this;
                }

                Iterator &operator-(USize n) {
                    IteratorBase<typename std::vector<T>::iterator>::handle -= n;
                    return *this;
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

                ReverseIterator &operator+(USize n) {
                    IteratorBase<typename std::vector<T>::reverse_iterator>::handle += n;
                    return *this;
                }

                ReverseIterator &operator-(USize n) {
                    IteratorBase<typename std::vector<T>::reverse_iterator>::handle -= n;
                    return *this;
                }
            };
            struct ConstIterator : IteratorBase<typename std::vector<T>::const_iterator> {
                ConstIterator(const typename std::vector<T>::const_iterator &it) : IteratorBase<typename std::vector<T>::const_iterator>(it) {

                }

                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_iterator>::handle);
                }

                ConstIterator &operator+(USize n) {
                    IteratorBase<typename std::vector<T>::const_iterator>::handle += n;
                    return *this;
                }

                ConstIterator &operator-(USize n) {
                    IteratorBase<typename std::vector<T>::const_iterator>::handle -= n;
                    return *this;
                }
            };
            struct ReverseConstIterator : IteratorBase<typename std::vector<T>::const_reverse_iterator> {
                ReverseConstIterator(const typename std::vector<T>::const_reverse_iterator &it) : IteratorBase<typename std::vector<T>::const_reverse_iterator>(it) {

                }

                const T &operator*() const {
                    return (*IteratorBase<typename std::vector<T>::const_reverse_iterator>::handle);
                }

                ReverseConstIterator &operator+(USize n) {
                    IteratorBase<typename std::vector<T>::const_reverse_iterator>::handle += n;
                    return *this;
                }

                ReverseConstIterator &operator-(USize n) {
                    IteratorBase<typename std::vector<T>::const_reverse_iterator>::handle -= n;
                    return *this;
                }
            };

            /// Initialize empty
            Array() {

            }

            /// Initialize with initial size
            /// \param size
            Array(USize size) : handle(size) {
            }

            /// Initialize from initializer list
            /// \param lst
            Array(const std::initializer_list<T> &lst) : handle(lst) {

            }

            /// Initialize from array
            /// \tparam N
            /// \param array
            template<USize N>
            Array(T(&&array)[N]) : handle(std::make_move_iterator(std::begin(array)), std::make_move_iterator(std::end(array))) {
                // ...
            }

            /// Convert to array view
            /// \return
            operator ArrayView<T>() {
                return ArrayView(Ptr(), Size());
            }

            /// Resize this array to size
            /// \param size
            DJINN_INLINE void Resize(USize size) {
                handle.resize(size);
            }

            /// Reserve this array to resize, does not change insertion index
            /// \param size
            DJINN_INLINE void Reserve(USize size) {
                handle.reserve(size);
            }

            /// Resize this array if needed, avoids reallocations
            /// \param size
            DJINN_INLINE void ResizePassive(USize size) {
                if (size > handle.size()) {
                    handle.resize(size);
                }
            }

            /// Add an element to the back of this array
            /// \param value
            DJINN_INLINE void Add(T &&value) {
                handle.push_back(value);
            }

            /// Add an element to the back of this array
            /// \param value
            DJINN_INLINE void Add(const T &value) {
                handle.push_back(value);
            }

            /// Emplace at the back of this array
            /// \param args
            /// \return
            template<typename... A>
            DJINN_INLINE T& Emplace(A&&... args) {
                return handle.emplace_back(args...);
            }

            /// Add an element at a sorted key value
            /// \param value
            /// \param key
            template<typename F>
            DJINN_INLINE void AddSorted(const T &value, F &&key) {
                auto it = std::upper_bound(handle.begin(), handle.end(), value, key);
                return handle.insert(it, value);
            }

            /// Add an element at a sorted key value
            /// \param value
            /// \param index - Insertion index
            /// \param key
            template<typename F>
            DJINN_INLINE void AddSorted(const T &value, USize& index, F &&key) {
                auto it = std::upper_bound(handle.begin(), handle.end(), value, key);
                index = std::distance(handle.begin(), it);
                return handle.insert(it, value);
            }

            /// Add another array to this array
            /// \param array
            DJINN_INLINE void Add(const Array &array) {
                handle.insert(handle.end(), array.begin().handle, array.end().handle);
            }

            /// Add range to this array
            /// \param array
            template<typename IT>
            DJINN_INLINE void AddRange(IT&& begin, IT&& end) {
                handle.insert(handle.end(), begin, end);
            }

            /// Add multiple values
            template<typename A0, typename A1, typename... AX>
            DJINN_INLINE void Add(const A0 &a0, const A1 &a1, const AX &... ax) {
                Add(a0);
                Add(a1, ax...);
            }

            /// Add a value to the front of this array
            /// \param value
            DJINN_INLINE void AddFront(T &&value) {
                handle.insert(handle.begin(), value);
            }

            /// Remove by iterator
            /// \param it
            /// \return - Returns true if successful
            DJINN_INLINE bool Remove(const Iterator &it) {
                handle.erase(it.handle);
                return true;
            }

            /// Remove by iterator
            /// \param it
            /// \return - Returns true if successful
            DJINN_INLINE bool Remove(const ReverseIterator &it) {
                handle.erase(std::next(it.handle).base());
                return true;
            }

            /// Remove by index value
            /// \param it
            /// \return - Returns true if successful
            DJINN_INLINE bool Remove(USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                handle.erase(handle.begin() + index);
                return true;
            }

            /// Remove all elements in range
            /// \param it
            /// \return - Returns true if successful
            DJINN_INLINE bool Remove(USize start, USize end) {
                DJINN_ASSERT(start < Size() && end <= Size(), "Array index out of bounds");
                DJINN_ASSERT(end > start, "Invalid erase range");
                handle.erase(handle.begin() + start, handle.begin() + end);
                return true;
            }

            /// Remove by value
            /// \param it
            /// \return - Returns true if successful
            DJINN_INLINE bool RemoveValue(const T &value) {
                bool result = false;
                handle.erase(std::remove_if(handle.begin(), handle.end(), [&](auto &&e) {
                    if (e == value) {
                        result = true;
                        return true;
                    }
                    return false;
                }), handle.end());
                return result;
            }

            /// Checks if this array contains a value
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

            /// Checks if this array contains a value and outputs index of given value
            /// \param value
            /// \param index
            /// \return
            DJINN_INLINE bool Contains(const T &value, USize &index) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (handle[i] == value) {
                        index = i;
                        return true;
                    }
                }
                return false;
            }

            /// Check if any element fits given predecate
            /// \tparam FUNCTOR
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

            /// Check if any element fits given predecate and outputs the index of the element
            /// \tparam FUNCTOR
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor, USize &index) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        index = i;
                        return true;
                    }
                }
                return false;
            }

            /// Apply predecate to all values
            /// \tparam FUNCTOR
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE void Foreach(FUNCTOR functor) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    functor(handle[i]);
                }
            }

            /// Returns true if predecate returns true for all values
            /// \tparam FUNCTOR
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE bool All(FUNCTOR functor) const {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (!functor(handle[i])) {
                        return false;
                    }
                }
                return true;
            }

            /// Remove all values matching a predicate
            /// \tparam FUNCTOR
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE void RemoveAll(FUNCTOR functor) {
                handle.erase(std::remove_if(handle.begin(), handle.end(), functor), handle.end());
            }

            /// Get first element which matches predecate or returns default
            /// \param functor
            /// \param _default
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE T FirstOrDefault(FUNCTOR functor, const T &_default = T()) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        return handle[i];
                    }
                }
                return _default;
            }

            /// Get address of first element which matches predecate or returns nullptr
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE T* FirstNullable(FUNCTOR functor) {
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    if (functor(handle[i])) {
                        return &handle[i];
                    }
                }
                return nullptr;
            }

            /// Select values based on predecate into new array
            /// \tparam R
            /// \param functor
            /// \return
            template<typename FUNCTOR, typename R = typename LambdaFunctionTraits<FUNCTOR>::ResultType>
            DJINN_INLINE Array<R> Select(FUNCTOR functor) {
                Array<R> result;
                for (Int64 i = handle.size() - 1; i >= 0; i--) {
                    result.Add(functor(handle[i]));
                }
                return result;
            }

            /// Sort this array based on comparator predecate
            /// \param comparator
            template<typename FUNCTOR>
            DJINN_INLINE void Sort(FUNCTOR comparator) {
                std::sort(handle.begin(), handle.end(), [&](auto &a, auto &b) {
                    return comparator(a, b);
                });
            }

            /// Get the size of this array
            /// \return
            DJINN_INLINE USize Size() const {
                return handle.size();
            }

            /// Get the byte size of this array
            /// \return
            DJINN_INLINE USize ByteSize() const {
                return handle.size() * sizeof(T);
            }

            /// Clear this array
            /// \param eraseMemory - If true forces the memory to be freed
            DJINN_INLINE void Clear(bool eraseMemory = false) {
                handle.clear();
                if (eraseMemory) {
                    handle.shrink_to_fit();
                }
            }

            /// Pop the last element of this array
            /// \return
            DJINN_INLINE T PopEnd() {
                T value = handle.back();
                handle.pop_back();
                return value;
            }

            /// Is this array empty?
            /// \return
            DJINN_INLINE bool Empty() const {
                return handle.empty();
            }

            /// Get the element at index
            /// \param index
            /// \return
            DJINN_INLINE T &Get(USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get the element at index
            /// \param index
            /// \return
            DJINN_INLINE T &operator[](USize index) {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get the element at index
            /// \param index
            /// \return
            DJINN_INLINE const T &operator[](USize index) const {
                DJINN_ASSERT(index < Size(), "Array index out of bounds");
                return handle[index];
            }

            /// Get the first element
            /// \return
            DJINN_INLINE T &First() {
                return handle.front();
            }

            /// Get last element
            /// \return
            DJINN_INLINE T &Last() {
                return handle.back();
            }

            /// Get start iterator
            /// \return
            DJINN_INLINE Iterator Start() {
                return Iterator{handle.begin()};
            }

            /// Get reverse start iterator
            /// \return
            DJINN_INLINE ReverseIterator RStart() {
                return ReverseIterator{handle.rbegin()};
            }

            /// Get end iterator
            /// \return
            DJINN_INLINE Iterator End() {
                return Iterator{handle.end()};
            }

            /// Get end iterator
            /// \return
            DJINN_INLINE ReverseIterator REnd() {
                return ReverseIterator{handle.rend()};
            }

            /// Get start iterator
            /// \return
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator{handle.begin()};
            }

            /// Get start iterator
            /// \return
            DJINN_INLINE ReverseConstIterator RStart() const {
                return ReverseConstIterator{handle.rbegin()};
            }

            /// Get end iterator
            /// \return
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator{handle.end()};
            }

            /// Get end iterator
            /// \return
            DJINN_INLINE ReverseConstIterator REnd() const {
                return ReverseConstIterator{handle.rend()};
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

            // Backend handler
            std::vector<T> handle;
        };
    }
}