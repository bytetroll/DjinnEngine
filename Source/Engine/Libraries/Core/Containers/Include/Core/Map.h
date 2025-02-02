#pragma once

#include <map>
#include <Core/Common.h>
#include <unordered_map>
#include <Core/Template/Common.h>
#include <Core/Size.h>
#include "Pair.h"

namespace Djinn {
    namespace Core {
        /// A collection of key value pairs
        /// \tparam KEY
        /// \tparam VALUE
        template<class KEY, class VALUE>
        class Map {
        public:
            // Iterator
            template<typename ITT>
            struct IteratorBase {
                ITT handle;

                IteratorBase(const ITT& it) : handle(it) {

                }

                bool operator==(const IteratorBase &other) const {
                    return handle == other.handle;
                }

                bool operator!=(const IteratorBase &other) const {
                    return handle != other.handle;
                }

                IteratorBase &operator++() {
                    ++handle;
                    return *this;
                }

                IteratorBase &operator--() {
                    --handle;
                    return *this;
                }
            };
            struct Iterator : IteratorBase<typename std::map<KEY, VALUE>::iterator> {
                Iterator(const typename std::map<KEY, VALUE>::iterator& it) : IteratorBase<typename std::map<KEY, VALUE>::iterator>(it) {

                }

                Core::Pair<const KEY&, VALUE&> operator*() const {
                    auto&& it = IteratorBase<typename std::map<KEY, VALUE>::iterator>::handle;
                    return {it->first, it->second};
                }

                VALUE *operator->() {
                    return &(*IteratorBase<typename std::map<KEY, VALUE>::iterator>::handle).second;
                }
            };
            struct ReverseIterator : IteratorBase<typename std::map<KEY, VALUE>::reverse_iterator> {
                ReverseIterator(const typename std::map<KEY, VALUE>::reverse_iterator& it) : IteratorBase<typename std::map<KEY, VALUE>::reverse_iterator>(it) {

                }

                Core::Pair<const KEY&, VALUE&> operator*() const {
                    auto it = *IteratorBase<typename std::map<KEY, VALUE>::reverse_iterator>::handle;
                    return {it.first, it.second};
                }

                VALUE *operator->() {
                    return &(*IteratorBase<typename std::map<KEY, VALUE>::reverse_iterator>::handle).second;
                }
            };
            struct ConstIterator : IteratorBase<typename std::map<KEY, VALUE>::const_iterator> {
                ConstIterator(const typename std::map<KEY, VALUE>::const_iterator& it) : IteratorBase<typename std::map<KEY, VALUE>::const_iterator>(it) {

                }

                Core::Pair<const KEY&, const VALUE&> operator*() const {
                    auto it = *IteratorBase<typename std::map<KEY, VALUE>::const_iterator>::handle;
                    return {it.first, it.second};
                }
            };
            struct ReverseConstIterator : IteratorBase<typename std::map<KEY, VALUE>::const_reverse_iterator> {
                ReverseConstIterator(const typename std::map<KEY, VALUE>::const_reverse_iterator& it) : IteratorBase<typename std::map<KEY, VALUE>::const_reverse_iterator>(it) {

                }

                Core::Pair<const KEY&, const VALUE&> operator*() const {
                    auto it = *IteratorBase<typename std::map<KEY, VALUE>::const_reverse_iterator>::handle;
                    return {it.first, it.second};
                }
            };

            /// Add a key value pair
            /// \param k
            /// \param v
            DJINN_INLINE void Add(const KEY &k, const VALUE &v) {
                handle[k] = v;
            }

            /// Add other map to this
            /// \param other
            DJINN_INLINE void Add(const Map &other) {
                handle.insert(other.Start().handle, other.End().handle);
            }

            /// Remove by key
            /// \param k
            /// \return
            DJINN_INLINE bool Remove(const KEY &k) {
                auto It = handle.find(k);
                if (It == handle.end())
                    return false;
                handle.erase(It);
                return true;
            }

            /// Remove by iterator
            /// \param it
            DJINN_INLINE void Remove(const Iterator &it) {
                handle.erase(it.handle);
            }

            /// Clear this map
            DJINN_INLINE void Clear() {
                handle.clear();
            }

            /// Get number of pairs
            /// \return
            DJINN_INLINE USize Size() {
                return handle.size();
            }

            /// Check if this map contains a key
            /// \param k
            /// \return
            DJINN_INLINE bool Contains(const KEY &k) {
                return handle.find(k) != handle.end();
            }

            /// Find by key
            /// \param k
            /// \return
            DJINN_INLINE Iterator Find(const KEY &k) {
                return Iterator {handle.find(k)};
            }

            /// Find by key
            /// \param k
            /// \return
            DJINN_INLINE ConstIterator Find(const KEY &k) const {
                return ConstIterator {handle.find(k)};
            }

            // Start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator {handle.begin()};
            }

            // Start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator {handle.begin()};
            }

            // End iterator
            DJINN_INLINE Iterator End() {
                return Iterator {handle.end()};
            }

            // End iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator {handle.end()};
            }

            /// Get value with key
            /// \param k
            /// \return
            DJINN_INLINE VALUE &At(const KEY &k) {
                return handle[k];
            }

            /// Get value with key
            /// \param k
            /// \return
            DJINN_INLINE VALUE &operator[](const KEY &k) {
                return handle[k];
            }

            /// Check if any pair matches predecate
            /// \tparam FUNCTOR
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool Any(FUNCTOR functor) const {
                for (auto it = Start(); it != End(); ++it) {
                    if (functor(*it)) {
                        return true;
                    }
                }
                return false;
            }

            /// Check if all values matches predecate
            /// \tparam FUNCTOR
            /// \param functor
            /// \return
            template<typename FUNCTOR>
            DJINN_INLINE bool All(FUNCTOR functor) const {
                for (auto it = Start(); it != End(); ++it) {
                    if (!functor(*it)) {
                        return false;
                    }
                }
                return true;
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

        private:
            typename std::map<KEY, VALUE> handle;
        };
    }
}