#pragma once

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Assert.h>

namespace Djinn {
    namespace Core {
        template<class T>
        class ArrayView {
        public:
            // Iterator
            struct Iterator {
                T *handle;

                /// Comparison
                bool operator==(const Iterator &other) const { return handle == other.handle; }
                bool operator!=(const Iterator &other) const { return handle != other.handle; }

                /// Getters
                T &operator*() const { return *handle; }
                T *operator->() const { return handle; }
                T &Get() const { return *handle; }

                Iterator &operator++() {
                    handle++;
                    return *this;
                }

                Iterator &operator--() {
                    handle--;
                    return *this;
                }
            };
            struct ConstIterator {
                T *handle;

                /// Comparison
                bool operator==(const ConstIterator &other) const { return handle == other.handle; }
                bool operator!=(const ConstIterator &other) const { return handle != other.handle; }

                /// Getters
                const T &operator*() const { return *handle; }
                const T *operator->() const { return handle; }
                const T &Get() const { return *handle; }

                ConstIterator &operator++() {
                    handle++;
                    return *this;
                }

                ConstIterator &operator--() {
                    handle--;
                    return *this;
                }
            };

            /// Construct
            ArrayView() : data(0), size(0) {

            }

            /// Construct from fixed size array
            /// \param array
            template<USize N>
            ArrayView(T (&array)[N]) : data(array), size(N) {

            }

            /// Construct
            /// \param array
            /// \param size
            ArrayView(T *array, USize size) : data(array), size(size) {

            }

            /// Single element constructor
            ArrayView(T &element) : data(&element), size(1) {

            }

            // Contains value?
            DJINN_INLINE bool Contains(const T &&value) {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (data[i] == value) {
                        return true;
                    }
                }
                return false;
            }

            // Any predicate
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor) {
                for (Int64 i = size - 1; i >= 0; i--) {
                    if (functor(data[i])) {
                        return true;
                    }
                }
                return false;
            }

            // Size of array
            DJINN_INLINE USize Size() const {
                return size;
            }

            // Get element
            DJINN_INLINE T &Get(USize index) {
                DJINN_ASSERT(index < size, "Index out of bounds");
                return data[index];
            }

            // Get element
            DJINN_INLINE const T &Get(USize index) const {
                DJINN_ASSERT(index < size, "Index out of bounds");
                return data[index];
            }

            // Get element
            DJINN_INLINE T &operator[](USize index) {
                DJINN_ASSERT(index < size, "Index out of bounds");
                return data[index];
            }

            // Get element
            DJINN_INLINE const T &operator[](USize index) const {
                DJINN_ASSERT(index < size, "Index out of bounds");
                return data[index];
            }

            /// Get data pointer
            /// \return
            T *Ptr() const {
                return data;
            }

            // Get start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator{data};
            }

            // Get end iterator
            DJINN_INLINE Iterator End() {
                return Iterator{data + size};
            }

            // Get start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator{data};
            }

            // Get end iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator{data + size};
            }

            /// Range iterator
            DJINN_INLINE Iterator begin() {
                return Start();
            }

            /// Range iterator
            DJINN_INLINE Iterator end() {
                return End();
            }

            /// Range iterator
            DJINN_INLINE ConstIterator begin() const {
                return Start();
            }

            /// Range iterator
            DJINN_INLINE ConstIterator end() const {
                return End();
            }

            // ...
            T *data;
            USize size;
        };

        template<typename T>
        class AppendArrayView {
        public:
            AppendArrayView() {

            }

            AppendArrayView(const ArrayView<T>& view) : view(view) {

            }

            /// Add an element
            /// \param element
            void Add(const T& element) {
                view[size++] = element;
            }

            /// Get the view with the new size
            /// \return
            ArrayView<T> GetView() const {
                return {view.Ptr(), size};
            }

            /// Get the underlying view
            /// \return
            ArrayView<T> GetUnderlyingView() const {
                return view;
            }

            /// Get size
            /// \return
            USize GetSize() const {
                return size;
            }

        private:
            ArrayView<T> view;
            USize size = 0;
        };
    }
}