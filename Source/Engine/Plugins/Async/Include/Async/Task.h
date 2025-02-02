//
// Created by Dev on 1/9/2018.
//
#pragma once

#include <cstddef>
#include <Core/SharedPtr.h>
#include <Async/Detail/AsyncHelper.h>
#include "Info.h"
#include "TaskObjectPool.h"
#include "Group.h"
#include "Framework.h"

namespace Djinn::Async {
    template<typename T>
    class Task {
    public:
        Task(std::nullptr_t = nullptr) {};

        /// Construct from conditional counter info
        /// \param info
        Task(typename TaskObjectPool<T>::Object *object) : object(object), jobGroup(&object->group) {
            object->AddUsage();
        }

        /// Construct from conditional counter info
        /// \param info
        Task(typename TaskObjectPool<T>::Object *object, JobGroup *jobGroup) : object(object), jobGroup(jobGroup) {
            object->AddUsage();
        }

        /// Copy
        /// \param other
        Task(const Task& other) : done(other.done), object(other.object), jobGroup(other.jobGroup) {
            object->AddUsage();
        }

        /// Deconstruct
        ~Task() {
            if (object) {
                object->ReleaseUsage();
            }
        }

        /// Assign operator
        /// \param other
        /// \return
        Task operator=(const Task& other) {
            if (object) {
                object->ReleaseUsage();
            }
            done = other.done;
            object = other.object;
            jobGroup = other.jobGroup;
            object->AddUsage();
            return *this;
        }

        /// Get value, may yield
        /// \return
        T &GetValue() {
            __Yield();
            return *object->Get();
        }

        /// Get value, may yield
        /// \return
        const T &GetValue() const {
            __Yield();
            return *object->Get();
        }

        /// Get raw value
        /// \return
        T &GetRawValue() {
            return *object->Get();
        }

        /// Get raw value
        /// \return
        const T &GetRawValue() const {
            return *object->Get();
        }

        /// Get value, may yield
        /// \return
        operator T &() {
            __Yield();
            return *object->Get();
        }

        /// Get value, may yield
        /// \return
        operator const T &() const {
            __Yield();
            return *object->Get();
        }

        /// Has this task completed?
        /// \return
        bool HasCompleted() const {
            return jobGroup->IsDone();
        }

        /// Wait for this task to complete
        void Wait() const {
            __Yield();
        }

        /// Wait for the result to become available
        /// Should not be called manually
        void __Yield() const {
            if (!HasCompleted()) {
                Detail::GetFramework()->Yield(jobGroup);
            }
        }

    private:
        mutable bool done = false;
        JobGroup *jobGroup = nullptr;
        typename TaskObjectPool<T>::Object *object = nullptr;
    };

    template<>
    class Task<void> {
    public:
        Task(std::nullptr_t = nullptr) {};

        /// Construct from conditional counter info
        /// \param info
        Task(typename TaskObjectPool<void>::Object *object) : object(object), jobGroup(&object->group) {
            object->AddUsage();
        }

        /// Construct from conditional counter info
        /// \param info
        Task(typename TaskObjectPool<void>::Object *object, JobGroup *jobGroup) : object(object), jobGroup(jobGroup) {
            object->AddUsage();
        }

        /// Copy
        /// \param other
        Task(const Task& other) : done(other.done), object(other.object), jobGroup(other.jobGroup) {
            object->AddUsage();
        }

        /// Deconstruct
        ~Task() {
            if (object) {
                object->ReleaseUsage();
            }
        }

        /// Assign operator
        /// \param other
        /// \return
        Task operator=(const Task& other) {
            if (object) {
                object->ReleaseUsage();
            }
            done = other.done;
            object = other.object;
            jobGroup = other.jobGroup;
            object->AddUsage();
            return *this;
        }

        /// Has this task completed?
        /// \return
        bool HasCompleted() const {
            return jobGroup->IsDone();
        }

        /// Wait for this task to complete
        void Wait() const {
            __Yield();
        }

        /// Wait for the result to become available
        /// Should not be called manually
        void __Yield() const {
            if (!HasCompleted()) {
                Detail::GetFramework()->Yield(jobGroup);
            }
        }

    private:
        mutable bool done = false;
        JobGroup *jobGroup = nullptr;
        typename TaskObjectPool<void>::Object *object = nullptr;
    };
}
