#pragma once

#include <Core/Common.h>
#include <string>
#include <Core/Atomic.h>
#include <Core/SharedPtr.h>
#include <sstream>
#include "Size.h"

namespace Djinn {
    namespace Core {
        class __oldString {
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
            struct Iterator : IteratorBase<typename std::string::iterator> {
                char &operator*() const {
                    return (*IteratorBase<typename std::string::iterator>::handle);
                }

                char *operator->() {
                    return &(*IteratorBase<typename std::string::iterator>::handle);
                }

                Iterator &operator++() {
                    handle++;
                    return *this;
                }

                Iterator operator+(int n) {
                    return Iterator {handle + n};
                }

                Iterator &operator--() {
                    handle--;
                    return *this;
                }

                Iterator operator-(int n) {
                    return Iterator {handle - n};
                }
            };
            struct ReverseIterator : IteratorBase<typename std::string::reverse_iterator> {
                char &operator*() const {
                    return *IteratorBase<typename std::string::reverse_iterator>::handle;
                }

                char *operator->() {
                    return &(*IteratorBase<typename std::string::reverse_iterator>::handle);
                }
            };
            struct ConstIterator : IteratorBase<typename std::string::const_iterator> {
                const char &operator*() const {
                    return (*IteratorBase<typename std::string::const_iterator>::handle);
                }

                ConstIterator &operator++() {
                    handle++;
                    return *this;
                }

                ConstIterator operator+(int n) {
                    return ConstIterator {handle + n};
                }

                ConstIterator &operator--() {
                    handle--;
                    return *this;
                }

                ConstIterator operator-(int n) {
                    return ConstIterator {handle - n};
                }
            };
            struct ReverseConstIterator : IteratorBase<typename std::string::const_reverse_iterator> {
                const char &operator*() const {
                    return (*IteratorBase<typename std::string::const_reverse_iterator>::handle);
                }
            };

            DJINN_INLINE __oldString() : handle("") {
                // ...
            }

            DJINN_INLINE __oldString(char ch) : handle(1, ch) {

            }

            DJINN_INLINE __oldString(const char *str) : handle(str) {
                // ...
            }

            const char *Ptr() const {
                return (handle).c_str();
            }

            char *Ptr() {
                return &(handle)[0];
            }

            void Add(char ch) {
                handle.push_back(ch);
            }

            void Add(const __oldString &str) {
                handle.insert(str.handle.end(), str.handle.begin(), str.handle.end());
            }

            __oldString operator+(const __oldString &r) const {
                return handle + r.handle;
            }

            __oldString& operator+=(const __oldString &r) {
                handle += r.handle;
                return *this;
            }

            USize Size() const {
                return handle.size();
            }

            bool operator==(const __oldString &other) const {
                return handle == other.handle;
            }

            bool operator<(const __oldString &other) const {
                return handle < other.handle;
            }

            bool operator>(const __oldString &other) const {
                return handle > other.handle;
            }

            template<class T>
            static __oldString From(const T &value) {
                std::stringstream ss;
                ss << value;
                return __oldString(ss.str());
            }

            char operator[](USize i) const {
                return handle[i];
            }

            // Resize string
            void Resize(USize n) {
                return handle.resize(n);
            }

            // Reserve string
            void Reserve(USize n) {
                return handle.reserve(n);
            }

            // Remove all occurances of
            static __oldString Remove(const __oldString &other, char ch) {
                __oldString str;
                str.Reserve(other.Size());

                Int64 n = 0;
                while (n < (Int64)other.Size()) {
                    Int64 next = other.IndexOf(n, ch);
                    if (next == -1) {
                        str.Add(str.End(), other.Start() + n, other.End());
                        break;
                    }
                    str.Add(str.End(), other.Start() + n, other.Start() + next);
                    n = next + 1;
                }

                return str;
            }

            // Add range
            void Add(const Iterator &slot, const Iterator &start, const Iterator &end) {
                handle.insert(slot.handle, start.handle, end.handle);
            }

            // Add range
            void Add(const Iterator &slot, const ConstIterator &start, const ConstIterator &end) {
                handle.insert(slot.handle, start.handle, end.handle);
            }

            // Index of
            USize IndexOf(char ch) const {
                return handle.find(ch);
            }

            // Index of
            Int64 IndexOf(USize offset, char ch) const {
                return handle.find(ch, offset);
            }

            // Get start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator {handle.begin()};
            }

            // Get reverse start iterator
            DJINN_INLINE ReverseIterator RStart() {
                return ReverseIterator {handle.rbegin()};
            }

            // Get end iterator
            DJINN_INLINE Iterator End() {
                return Iterator {handle.end()};
            }

            // Get end iterator
            DJINN_INLINE ReverseIterator REnd() {
                return ReverseIterator {handle.rend()};
            }

            // Get start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator {handle.begin()};
            }

            // Get start iterator
            DJINN_INLINE ReverseConstIterator RStart() const {
                return ReverseConstIterator {handle.rbegin()};
            }

            // Get end iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator {handle.end()};
            }

            // Get end iterator
            DJINN_INLINE ReverseConstIterator REnd() const {
                return ReverseConstIterator {handle.rend()};
            }

            // Range expressions
            DJINN_INLINE Iterator begin() {
                return Start();
            }

            DJINN_INLINE ConstIterator begin() const {
                return Start();
            }

            DJINN_INLINE Iterator end() {
                return End();
            }

            DJINN_INLINE ConstIterator end() const {
                return End();
            }

        private:
            __oldString(const std::string &__Handle) : handle(__Handle) {

            }

            std::string handle;
        };

        static __oldString operator+(const char *l, const __oldString &r) {
            return __oldString(l) + r;
        }

        static __oldString operator+(const __oldString &l, const char *r) {
            return l + __oldString(r);
        }
    }
}