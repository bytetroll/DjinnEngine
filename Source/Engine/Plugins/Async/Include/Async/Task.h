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
