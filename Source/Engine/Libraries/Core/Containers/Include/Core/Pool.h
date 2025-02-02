//
// Created by Dev on 11/22/2017.
//
#pragma once

#include "Array.h"

namespace Djinn::Core {
    template<typename T>
    class ObjectPool {
    public:
        /// Pop object from this pool
        /// \return
        template <typename... A>
        T* Pop(A&&... args) {
            if (objects.Size() == 0) {
                return new T(args...);
            }
            return objects.PopEnd();
        }

        /// Pop object from this pool
        /// \return
        template <typename... A>
        T* PopConstruct(A&&... args) {
            if (objects.Size() == 0) {
                return new T(args...);
            }
            T* obj = objects.PopEnd();
            obj->~T();
            new (obj) T(args...);
            return obj;
        }

        /// Pop object from this pool without creation
        /// \return
        T* PopOrNull() {
            if (objects.Size() == 0) {
                return nullptr;
            }
            return objects.PopEnd();
        }

        /// Push object to this pool
        /// \param obj
        void Push(T* obj) {
            objects.Add(obj);
        }

    private:
        Array<T*> objects;
    };

    template<typename T>
    class LazyObjectPool {
    public:
        /// Pop object from this pool
        /// \return
        template <typename... A>
        T* Pop(A&&... args) {
            T* ptr = pool.Pop(args...);
            objects.Add(ptr);
            return ptr;
        }

        /// Pop object from this pool
        /// \return
        template <typename... A>
        T* PopConstruct(A&&... args) {
            T* ptr = pool.PopConstruct(args...);
            objects.Add(ptr);
            return ptr;
        }

        /// Pop object from this pool without creation
        /// \return
        T* PopOrNull() {
            T* ptr = pool.PopOrNull();
            objects.Add(ptr);
            return ptr;
        }

        /// Flush all the popped objects
        void Flush() {
            for (auto& ptr : objects) {
                pool.Push(ptr);
            }
            objects.Clear(false);
        }

        /// Get current pooled objects
        /// \return
        Array<T*>& GetObjects() {
            return objects;
        }

        /// Get current pooled objects
        /// \return
        const Array<T*>& GetObjects() const {
            return objects;
        }

    private:
        Array<T*> objects;
        ObjectPool<T> pool;
    };
}
