//
// Created by Dev on 1/9/2018.
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
