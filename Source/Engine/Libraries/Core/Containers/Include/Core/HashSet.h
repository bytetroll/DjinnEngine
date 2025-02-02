#pragma once

#include <set>
#include <Core/Common.h>
#include <Core/Hash.h>
#include <Core/Comparator.h>
#include <unordered_set>
#include <Core/Template/Common.h>
#include <Core/Size.h>

namespace Djinn {
    namespace Core {
        /// A collection of values with hashed lookup
        /// \tparam VALUE
        template <class VALUE>
        class HashSet {
        public:
            using Type = std::unordered_set<VALUE, Hasher<VALUE>, Comparator<VALUE>>;

            // Iterator
            struct Iterator {
                typename Type::iterator handle;
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
                VALUE operator*() const {
                    return *handle;
                }
            };
            struct ConstIterator {
                typename Type::const_iterator handle;
                bool operator==(const ConstIterator& other) {
                    return handle == other.handle;
                }
            };

            /// Add a value
            /// \param v
            DJINN_INLINE void Add(const VALUE& v) {
                handle.insert(v);
            }

            /// Remove a value
            /// \param k
            /// \return
            DJINN_INLINE bool Remove(const VALUE& k) {
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

            /// Get number of value
            /// \return
            DJINN_INLINE USize Size() {
                return handle.size();
            }

            /// Clear this set
            DJINN_INLINE void Clear() {
                handle.clear();
            }

            /// Check if this set contains value
            /// \param k
            /// \return
            DJINN_INLINE bool Contains(const VALUE& k) {
                return handle.find(k) != handle.end();
            }

            /// Find value
            /// \param k
            /// \return
            DJINN_INLINE Iterator Find(const VALUE& k) {
                return Iterator {  handle.find(k) };
            }

            /// Find value
            /// \param k
            /// \return
            DJINN_INLINE ConstIterator Find(const VALUE& k) const {
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

            /// Range expression
            DJINN_INLINE Iterator begin() {
                return Start();
            }

            /// Range expression
            DJINN_INLINE Iterator end() {
                return End();
            }

        private:
            Type handle;
        };
    }
}