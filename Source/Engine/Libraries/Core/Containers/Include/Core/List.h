#pragma once

#include <list>
#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Template/Lambda.h>
#include <Core/Size.h>

namespace Djinn::Core {
    /// A non continuous collection of elements
    /// \tparam T
    template<class T>
    class List {
    public:
        // Iterator
        template<typename ITT>
        struct IteratorBase {
            ITT handle;

            bool operator==(const IteratorBase &other) const {
                return handle == other.handle;
            }

            bool operator!=(const IteratorBase &other) const {
                return handle != other.handle;
            }
        };

        struct Iterator : IteratorBase<typename std::list<T>::iterator> {
            T &operator*() const {
                return (*IteratorBase<typename std::list<T>::iterator>::handle);
            }

            T *operator->() {
                return &(*IteratorBase<typename std::list<T>::iterator>::handle);
            }

            Iterator &operator++() {
                IteratorBase<typename std::list<T>::iterator>::handle++;
                return *this;
            }

            Iterator &operator--() {
                IteratorBase<typename std::list<T>::iterator>::handle--;
                return *this;
            }
        };

        struct ReverseIterator : IteratorBase<typename std::list<T>::reverse_iterator> {
            T &operator*() const {
                return *IteratorBase<typename std::list<T>::reverse_iterator>::handle;
            }

            T *operator->() {
                return &(*IteratorBase<typename std::list<T>::reverse_iterator>::handle);
            }

            ReverseIterator &operator++() {
                IteratorBase<typename std::list<T>::iterator>::handle++;
                return *this;
            }

            ReverseIterator &operator--() {
                IteratorBase<typename std::list<T>::iterator>::handle--;
                return *this;
            }
        };

        struct ConstIterator : IteratorBase<typename std::list<T>::const_iterator> {
            const T &operator*() const {
                return (*IteratorBase<typename std::list<T>::const_iterator>::handle);
            }
        };

        struct ReverseConstIterator : IteratorBase<typename std::list<T>::const_reverse_iterator> {
            const T &operator*() const {
                return (*IteratorBase<typename std::list<T>::const_reverse_iterator>::handle);
            }
        };

        List() {

        }

        /// Initialize from initializer list
        /// \param lst
        List(const std::initializer_list<T> &lst) : handle(lst) {

        }

        /// Initialize from List
        /// \tparam N
        /// \param List
        template<USize N>
        List(T(&&List)[N]) : handle(std::make_move_iterator(std::begin(List)), std::make_move_iterator(std::end(List))) {
            // ...
        }

        /// Resize this list
        /// \param size
        DJINN_INLINE void Resize(USize size) {
            handle.resize(size);
        }

        /// Resize this list if needed
        /// \param size
        DJINN_INLINE void ResizePassive(USize size) {
            if (size > handle.size()) {
                handle.resize(size);
            }
        }

        /// Add to list before iterator
        /// \param it
        /// \param value
        /// \return
        DJINN_INLINE Iterator Add(Iterator it, const T &value) {
            return Iterator{handle.insert(it.handle, value)};
        }

        /// Emplace at the back of this list
        /// \param args
        /// \return
        template<typename... A>
        DJINN_INLINE T& Emplace(A&&... args) {
            return handle.emplace_back(args...);
        }

        /// Add to the back of this list
        /// \param value
        /// \return
        DJINN_INLINE Iterator Add(T &&value) {
            handle.push_back(value);
            return --End();
        }

        /// Add to the back of this list
        /// \param value
        /// \return
        DJINN_INLINE Iterator Add(const T &value) {
            handle.push_back(value);
            return --End();
        }

        /// Add another list to this list
        /// \param List
        DJINN_INLINE void Add(const List &List) {
            handle.insert(handle.end(), List.begin().handle, List.end().handle);
        }

        /// Add to the front of this array
        /// \param value
        /// \return
        DJINN_INLINE Iterator AddFront(T &&value) {
            return Iterator{handle.insert(handle.begin(), value)};
        }

        /// Remove by iterator
        /// \param it
        /// \return
        DJINN_INLINE bool Remove(const Iterator &it) {
            handle.erase(it.handle);
            return true;
        }

        /// Remove by iterator
        /// \param it
        /// \return
        DJINN_INLINE bool Remove(const ReverseIterator &it) {
            handle.erase(std::next(it.handle).base());
            return true;
        }

        /// Remove by index
        /// \param it
        /// \return
        DJINN_INLINE bool Remove(USize index) {
            DJINN_ASSERT(index < Size(), "List index out of bounds");
            handle.erase(handle.begin() + index);
            return true;
        }

        /// Remove by value
        /// \param it
        /// \return
        DJINN_INLINE bool RemoveValue(const T &value) {
            handle.remove_if([&](auto it) {
                return it == value;
            });
            return false;
        }

        /// Check if this list contains
        /// \param value
        /// \return
        DJINN_INLINE bool Contains(const T &value) {
            for (auto &it : handle) {
                if (it == value) {
                    return true;
                }
            }
            return false;
        }

        /// Check if any element matches predecate
        /// \param functor
        /// \return
        template<typename FUNCTOR>
        DJINN_INLINE bool Any(FUNCTOR functor) const {
            for (auto &it : handle) {
                if (functor(it)) {
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
            for (auto &it : handle) {
                if (!functor(it)) {
                    return false;
                }
            }
            return true;
        }

        /// Remove all values matching predecate
        /// \param functor
        template<typename FUNCTOR>
        DJINN_INLINE void RemoveAll(FUNCTOR functor) {
            handle.remove_if([&](auto it) {
                return functor(it);
            });
        }

        /// Select all values matching predecate into new list
        /// \param functor
        /// \return
        template<typename FUNCTOR, typename R = typename LambdaFunctionTraits<FUNCTOR>::ResultType>
        DJINN_INLINE List<R> Select(FUNCTOR functor) {
            List<R> result;
            for (auto &it : handle) {
                result.Add(functor(it));
            }
            return result;
        }

        /// Get size of this list
        /// \return
        DJINN_INLINE USize Size() const {
            return handle.size();
        }

        /// Clear this list
        DJINN_INLINE void Clear() {
            handle.clear();
        }

        /// Pop front value
        /// \return
        DJINN_INLINE T PopFront() {
            DJINN_ASSERT(Size() > 0, "Popping first element of empty list");
            T value = handle.front();
            handle.pop_front();
            return value;
        }

        /// Pop last value
        /// \return
        DJINN_INLINE T PopEnd() {
            DJINN_ASSERT(Size() > 0, "Popping last element of empty list");
            T value = handle.back();
            handle.pop_back();
            return value;
        }

        /// Get first element
        /// \return
        DJINN_INLINE T &First() {
            DJINN_ASSERT(Size() > 0, "Getting first element of empty list");
            return handle.front();
        }

        /// Get last element
        /// \return
        DJINN_INLINE T &Last() {
            DJINN_ASSERT(Size() > 0, "Getting last element of empty list");
            return handle.back();
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
            return ReverseIterator{handle.rend()};
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
            return ConstIterator{handle.end()};
        }

        // Get end iterator
        DJINN_INLINE ReverseConstIterator REnd() const {
            return ReverseConstIterator{handle.rend()};
        }

        /// Get data pointer
        /// \return
        DJINN_INLINE T *Ptr() {
            return &handle.front();
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
        std::list<T> handle;
    };
}
