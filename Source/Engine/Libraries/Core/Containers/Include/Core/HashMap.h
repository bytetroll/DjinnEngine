#pragma once

#include <map>
#include <Core/Common.h>
#include <Core/Hash.h>
#include <Core/Comparator.h>
#include <unordered_map>
#include <Core/Template/Common.h>
#include <Core/Size.h>
#include "Pair.h"

namespace Djinn {
    namespace Core {
        /// A collection of key value pairs with hashed lookup
        /// \tparam KEY
        /// \tparam VALUE
        template<class KEY, class VALUE>
        class HashMap {
        public:
            using Type = std::unordered_map<KEY, VALUE, Hasher<KEY>, Comparator<KEY>>;

            // Iterator
            template<typename ITT>
            struct IteratorBase {
                ITT handle;

                IteratorBase(const ITT &it) : handle(it) {

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
            struct Iterator : IteratorBase<typename Type::iterator> {
                Iterator(const typename Type::iterator &it) : IteratorBase<typename Type::iterator>(it) {

                }

                Core::Pair<const KEY &, VALUE &> operator*() const {
                    auto &&it = IteratorBase<typename Type::iterator>::handle;
                    return {it->first, it->second};
                }

                VALUE *operator->() {
                    return &(*IteratorBase<typename Type::iterator>::handle).second;
                }
            };
            struct ConstIterator : IteratorBase<typename Type::const_iterator> {
                ConstIterator(const typename Type::const_iterator &it) : IteratorBase<typename Type::const_iterator>(it) {

                }

                Core::Pair<const KEY &, const VALUE &> operator*() const {
                    auto it = *IteratorBase<typename Type::const_iterator>::handle;
                    return {it.first, it.second};
                }
            };

            /// Add a new element
            /// \param k
            /// \param v
            DJINN_INLINE void Add(const KEY &k, const VALUE &v) {
                handle[k] = v;
            }

            /// Add another map to this map
            /// \param other
            DJINN_INLINE void Add(const HashMap &other) {
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

            /// Remove all elements matching predecate
            /// \tparam F
            /// \param action
            template<typename F>
            DJINN_INLINE void RemoveAll(F &&action) {
                for (auto it = handle.begin(); it != handle.end();) {
                    if (action(*it)) {
                        it = handle.erase(it);
                    } else {
                        ++it;
                    }
                }
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

            /// Get number of elements
            /// \return
            DJINN_INLINE USize Size() {
                return handle.size();
            }

            /// Check if this map contains given key
            /// \param k
            /// \return
            DJINN_INLINE bool Contains(const KEY &k) {
                return handle.count(k) != 0;
            }

            /// Get first value from key or default
            /// \param k
            /// \param value
            /// \return
            DJINN_INLINE VALUE GetOrDefault(const KEY &k, const VALUE &value) {
                auto it = Find(k);
                if (it == End()) {
                    return value;
                }
                return (*it).second;
            }

            /// Get first value from key or default
            /// \param k
            /// \param value
            /// \return
            DJINN_INLINE const VALUE &GetOrDefault(const KEY &k, const VALUE &value) const {
                auto it = Find(k);
                if (it == End()) {
                    return value;
                }
                return *(*it).second;
            }


            /// Get first address of value from key or null
            /// \param k
            /// \param value
            /// \return
            DJINN_INLINE VALUE *GetNullable(const KEY &k) {
                auto it = Find(k);
                if (it == End()) {
                    return nullptr;
                }
                return &(*it).second;
            }

            /// Find value by key
            /// \param k
            /// \return
            DJINN_INLINE Iterator Find(const KEY &k) {
                return Iterator{handle.find(k)};
            }

            /// Find value by key
            /// \param k
            /// \return
            DJINN_INLINE ConstIterator Find(const KEY &k) const {
                return ConstIterator{handle.find(k)};
            }

            // Start iterator
            DJINN_INLINE Iterator Start() {
                return Iterator{handle.begin()};
            }

            // Start iterator
            DJINN_INLINE ConstIterator Start() const {
                return ConstIterator{handle.begin()};
            }

            // End iterator
            DJINN_INLINE Iterator End() {
                return Iterator{handle.end()};
            }

            // End iterator
            DJINN_INLINE ConstIterator End() const {
                return ConstIterator{handle.end()};
            }

            /// Get by key
            /// \param k
            /// \return
            DJINN_INLINE VALUE &At(const KEY &k) {
                return handle[k];
            }

            /// Get by key
            /// \param k
            /// \return
            DJINN_INLINE VALUE &operator[](const KEY &k) {
                return handle[k];
            }

            /// Check if any element matches predecate
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

            /// Check if all elements matches predecate
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
            Type handle;
        };
    }
}