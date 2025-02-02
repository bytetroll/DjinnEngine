#pragma once

#include <map>
#include <Core/Common.h>
#include "Size.h"
#include "Pair.h"

namespace Djinn {
    namespace Core {
        /// A collection of key value pairs with multiple values per key
        /// \tparam KEY
        /// \tparam VALUE
        template <class KEY, class VALUE>
        class MultiMap {
        public:
            // Iterator
            struct Iterator {
                typename std::multimap<KEY, VALUE>::iterator handle;
                bool operator==(const Iterator& other) const {
                    return handle == other.handle;
                }
                bool operator!=(const Iterator& other) const {
                    return handle != other.handle;
                }
                Iterator& operator++() {
                    handle++;
                    return *this;
                }
                Pair<KEY, VALUE> operator*() const {
                    return Pair<KEY, VALUE>(handle->first, handle->second);
                }
            };
            struct ConstIterator {
                typename std::multimap<KEY, VALUE>::const_iterator handle;
                bool operator==(const ConstIterator& other) {
                    return handle == other.handle;
                }
            };

            /// Add key value pair
            /// \param k
            /// \param v
            DJINN_INLINE void Add(const KEY& k, const VALUE& v) {
                handle.insert(std::pair<KEY, VALUE>(k, v));
            }

            /// Add other multimap to this
            /// \param other
            DJINN_INLINE void Add(const MultiMap& other) {
                handle.insert(other.Start().handle, other.End().handle);
            }

            /// Remove by key
            /// \param k
            /// \return
            DJINN_INLINE bool Remove(const KEY& k) {
                auto It = handle.find(k);
                if (It == handle.end())
                    return false;
                handle.erase(It);
                return true;
            }

            /// Remove by iterator
            /// \param it
            DJINN_INLINE void Remove(const Iterator& it) {
                handle.erase(it.handle);
            }

            /// Get number of values
            /// \return
            DJINN_INLINE USize Size() {
                return handle.size();
            }

            /// Check if this map contains key
            /// \param k
            /// \return
            DJINN_INLINE bool Contains(const KEY& k) {
                return handle.count(k) != 0;
            }

            /// Find by key
            /// \param k
            /// \return
            DJINN_INLINE Iterator Find(const KEY& k) {
                return Iterator {  handle.find(k) };
            }

            /// Find by key
            /// \param k
            /// \return
            DJINN_INLINE ConstIterator Find(const KEY& k) const {
                return ConstIterator {  handle.find(k) };
            }

            // Start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator { handle.begin() };
            }

            // Start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator { handle.begin() };
            }

            // End iterator
            DJINN_INLINE Iterator End() {
                return Iterator { handle.end() };
            }

            // End iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator { handle.end() };
            }

            /// Get value by key
            /// \param k
            /// \return
            DJINN_INLINE VALUE& operator[](const KEY& k) {
                return handle.equal_range(k);
            }

            /// Range expression
            DJINN_INLINE Iterator begin() {
                return Start();
            }

            /// Range expression
            DJINN_INLINE Iterator end() {
                return End();
            }

        private:
            std::multimap<KEY, VALUE> handle;
        };
    }
}