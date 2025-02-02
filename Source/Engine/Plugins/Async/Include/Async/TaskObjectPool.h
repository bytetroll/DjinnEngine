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
 on 1/9/2018.
//
#pragma once

#include <Platform/LockGuard.h>
#include <Core/Array.h>
#include <Core/Atomic.h>
#include "JobGroup.h"

namespace Djinn::Async {
    template<typename T>
    class TaskObjectPool {
    public:
        struct Object {
            T* Get() {
                return reinterpret_cast<T*>(buffer);
            }

            void ReleaseUsage() {
                if (--usage == 0) {
                    self->Push(this);
                }
            }

            void AddUsage() {
                usage++;
            }

            TaskObjectPool* self;
            char buffer[sizeof(T)];
            JobGroup group;
            Core::AtomicCounter usage;
        };

        /// Pop object from this pool
        /// \return
        template<typename... A>
        Object *Pop(A &&... args) {
            Platform::LockGuard guard(mtx);
            if (objects.Size() == 0) {
                auto ptr = new Object();
                ptr->self = this;
                return ptr;
            }
            auto obj = objects.PopEnd();
            new (obj) Object();
            obj->self = this;
            return obj;
        }

        /// Push object to this pool
        /// \param obj
        void Push(Object *obj) {
            Platform::LockGuard guard(mtx);
            objects.Add(obj);
            auto ptr = reinterpret_cast<T*>(obj->buffer);
            ptr->~T();
        }

        /// Get instance
        /// \return
        static TaskObjectPool& Instance() {
            static TaskObjectPool instance;
            return instance;
        }

    private:
        Platform::SpinMutex mtx;
        Core::Array<Object *> objects;
    };

    template<>
    class TaskObjectPool<void> {
    public:
        struct Object {
            void ReleaseUsage() {
                if (--usage == 0) {
                    self->Push(this);
                }
            }

            void AddUsage() {
                usage++;
            }

            TaskObjectPool* self;
            JobGroup group;
            Core::AtomicCounter usage;
        };

        /// Pop object from this pool
        /// \return
        template<typename... A>
        Object *Pop(A &&... args) {
            Platform::LockGuard guard(mtx);
            if (objects.Size() == 0) {
                auto ptr = new Object();
                ptr->self = this;
                return ptr;
            }
            auto obj = objects.PopEnd();
            new (obj) Object();
            obj->self = this;
            return obj;
        }

        /// Push object to this pool
        /// \param obj
        void Push(Object *obj) {
            Platform::LockGuard guard(mtx);
            objects.Add(obj);
        }

        /// Get instance
        /// \return
        static TaskObjectPool& Instance() {
            static TaskObjectPool instance;
            return instance;
        }

    private:
        Platform::SpinMutex mtx;
        Core::Array<Object *> objects;
    };
}
