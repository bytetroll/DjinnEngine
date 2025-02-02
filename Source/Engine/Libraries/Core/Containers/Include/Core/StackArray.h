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

namespace Djinn {
    namespace Core {
        /// A collection of elements with a limited stack allocation and the heap as a fallback
        /// \tparam T
        /// \tparam SIZE
        template<class T, USize SIZE>
        class StackArray {
        public:
            // Iterator
            struct IteratorBase {
                T *handle;

                IteratorBase(T *t) : handle(t) {

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
            struct Iterator : public IteratorBase {
                Iterator(T *it) : IteratorBase(it) {

                }

                T &operator*() const {
                    return *IteratorBase::handle;
                }

                T *operator->() {
                    return IteratorBase::handle;
                }
            };
            struct ConstIterator : public IteratorBase {
                ConstIterator(T *it) : IteratorBase(it) {

                }

                const T &operator*() const {
                    return *IteratorBase::handle;
                }
            };

            /// Construct
            StackArray() {
                ptr = &handle[0];
            }

            /// Copy
            /// \param other
            StackArray(const StackArray &other) {
                size = other.size;
                if ((isFallback = other.isFallback)) {
                    fallback = other.fallback;
                    ptr = fallback.data();
                } else {
                    handle = other.handle;
                    ptr = &handle[0];
                }
            }

            /// Convert to array view
            /// \return
            operator ArrayView<T>() {
                return ArrayView(Ptr(), Size());
            }

            /// Copy assignment operator
            /// \return
            StackArray &operator=(const StackArray &other) {
                size = other.size;
                if ((isFallback = other.isFallback)) {
                    fallback = other.fallback;
                    ptr = fallback.data();
                } else {
                    handle = other.handle;
                    ptr = &handle[0];
                }
                return *this;
            }

            /// Add a value to the back of this array
            /// \param value
            DJINN_INLINE void Add(T &&value) {
                Add(value);
            }

            /// Add a value to the back of this array
            /// \param value
            DJINN_INLINE void Add(const T &value) {
                if (size < SIZE) {
                    handle[size++] = value;
                } else {
                    size++;
                    if (!isFallback) {
                        isFallback = true;
                        fallback.resize(size);
                        for (USize i = 0; i < size - 1; i++) {
                            fallback[i] = handle[i];
                        }
                    }
                    fallback.push_back(value);
                    ptr = fallback.data();
                }
            }

            /// Resize this array
            /// \param size
            DJINN_INLINE void Resize(USize size) {
                auto old = this->size;
                this->size = size;
                if (size >= SIZE) {
                    fallback.resize(size);
                    if (!isFallback) {
                        for (USize i = 0; i < old; i++) {
                            fallback[i] = handle[i];
                        }
                        isFallback = true;
                    }
                    ptr = fallback.data();
                }

            }

            /// Resize this array if needed
            /// \param size
            /// \return
            DJINN_INLINE bool ResizePassive(USize size) {
                if (this->size < size) {
                    Resize(size);
                    return true;
                }
                return false;
            }

            /// Remove from this array
            /// \param index
            DJINN_INLINE void Remove(USize index) {
                DJINN_ASSERT(index < Size(), "Stack array index out of bounds");
                if (isFallback) {
                    fallback.erase(fallback.begin() + index);
                } else {
                    for (USize i = index; i < size - 1; i++) {
                        handle[i] = handle[i + 1];
                    }
                }
                size--;
            }

            DJINN_INLINE void RemoveValue(const T &value) {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (ptr[i] == value) {
                        Remove(i);
                        break;
                    }
                }
            }

            /// Check if this stack array contains a value
            /// \param value
            /// \return
            DJINN_INLINE bool Contains(const T &value) {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (ptr[i] == value) {
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
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (functor(ptr[i])) {
                        return true;
                    }
                }
                return false;
            }

            /// Check if any value matches predecate
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor) {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (functor(ptr[i])) {
                        return true;
                    }
                }
                return false;
            }

            /// Check if any value matches predecate
            /// \tparam FUNCTOR
            /// \param functor
            /// \param index - Index of matched element
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor, USize &index) const {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (functor(ptr[i])) {
                        index = static_cast<USize>(i);
                        return true;
                    }
                }
                return false;
            }

            /// Iterate by predecate
            /// \param functor
            template<typename FUNCTOR>
            DJINN_INLINE void Foreach(FUNCTOR functor) const {
                for (Int64 i = size - 1; i >= 0; i--) {
                    functor(ptr[i]);
                }
            }

            /// Check if all elements matches predecate
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool All(FUNCTOR functor) const {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (!functor(ptr[i])) {
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
                    if (functor(ptr[i])) {
                        return ptr[i];
                    }
                }
                return _default;
            }

            /// Select values from this stack array by predecate
            /// \tparam R
            /// \param functor
            /// \return
            template<typename FUNCTOR, typename R = typename LambdaFunctionTraits<FUNCTOR>::ResultType>
            DJINN_INLINE StackArray<R, SIZE> Select(FUNCTOR functor) {
                StackArray<R, SIZE> result;
                for (Int64 i = size - 1; i >= 0; i--) {
                    result.Add(functor(ptr[i]));
                }
                return result;
            }

            /// Clear this stack array
            DJINN_INLINE void Clear() {
                size = 0;
            }

            /// Get the number of values
            /// \return
            DJINN_INLINE USize Size() const {
                return size;
            }

            /// Pop the last value
            /// \return
            DJINN_INLINE T PopEnd() {
                DJINN_ASSERT(Size() > 0, "Popping an empty stack array");
                size--;
                return ptr[size];
            }

            /// Get by index
            /// \param index
            /// \return
            DJINN_INLINE T &Get(USize index) {
                DJINN_ASSERT(index < Size(), "Stack array out of bounds");
                return ptr[index];
            }

            /// Get element
            /// \param index
            /// \return
            DJINN_INLINE T &operator[](USize index) {
                DJINN_ASSERT(index < Size(), "Stack array out of bounds");
                return ptr[index];
            }

            /// Get element
            /// \param index
            /// \return
            DJINN_INLINE const T &operator[](USize index) const {
                DJINN_ASSERT(index < Size(), "Stack array out of bounds");
                return ptr[index];
            }

            /// Get first element
            /// \return
            DJINN_INLINE T &First() {
                return ptr[0];
            }

            /// Get last element
            /// \return
            DJINN_INLINE T &Last() {
                return ptr[size - 1];
            }

            // Get start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator{ptr};
            }

            // Get end iterator
            DJINN_INLINE Iterator End() {
                return Iterator{ptr + size};
            }

            // Get start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator{ptr};
            }

            // Get end iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator{ptr + size};
            }

            // Get data pointer
            DJINN_INLINE T *Ptr() {
                return handle.data();
            }

            // Get data pointer
            DJINN_INLINE const T *Ptr() const {
                return ptr;
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
            bool isFallback = false;

            // Backend handlers
            std::array<T, SIZE> handle;
            std::vector<T> fallback;
            T *ptr = nullptr;
        };

        /// Common usages
        template<typename T> using StackArray4 = StackArray<T, 4>;
        template<typename T> using StackArray8 = StackArray<T, 8>;
        template<typename T> using StackArray12 = StackArray<T, 12>;
        template<typename T> using StackArray16 = StackArray<T, 16>;
        template<typename T> using StackArray32 = StackArray<T, 32>;
        template<typename T> using StackArray64 = StackArray<T, 64>;
        template<typename T> using StackArray128 = StackArray<T, 128>;
    }
}