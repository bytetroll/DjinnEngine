//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/22/2017.
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
