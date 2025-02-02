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
 on 1/4/2018.
//
#pragma once

#include <Async/ContextAPI.h>
#include "Detail/AsyncHelper.h"
#include <Core/Math.h>
#include "ContextData.h"
#include "Framework.h"
#include "Group.h"
#include "Task.h"
#include "IScheduler.h"

namespace Djinn::Async {
    /// Schedule helper
    /// \param info
    static void Schedule(UInt32 count, const ScheduledJob *jobs, const LaneID &lane = kDefault) {
        Detail::GetFramework()->Schedule(count, jobs, lane);
    }

    /// Schedule helper
    /// \param info
    static void Schedule(const Core::Delegate<void()> &delegate, const LaneID &lane = kDefault) {
        ScheduledJob job;
        job.isParameterized = false;
        job.info.delegate = delegate;
        Detail::GetFramework()->Schedule(1, &job, lane);
    }

    /// Schedule helper
    /// \param info
    template<typename R = void>
    static Task<R> ScheduleTask(const Core::Delegate<R()> &delegate, const LaneID &lane = kDefault) {
        // Allocate instance
        auto object = TaskObjectPool<R>::Instance().Pop();
        object->group.Increment();

        // Resulting task
        Task<R> task(object);

        // Schedule job
        ScheduledJob job;
        job.isParameterized = true;
        job.parameterizedInfo.stack = Core::Pack<Core::Delegate<R()>>(delegate);
        job.parameterizedInfo.delegate = [=](Core::Delegate<R()> _delegate) {
            if constexpr(Core::IsSame<void, R>::kValue) {
                _delegate.Invoke();
            } else {
                new(object->Get()) R(_delegate.Invoke());
            }
        };
        job.parameterizedInfo.delegate.SetRTTI(delegate);
        job.group = &object->group;
        job.post.argument = 1;
        job.post.op = CounterOp::eAdd;
        Detail::GetFramework()->Schedule(1, &job, lane);
        return task;
    }

    /// Schedule helper
    /// \param info
    static void Schedule(const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack, const LaneID &lane = kDefault) {
        ScheduledJob job;
        job.isParameterized = true;
        job.parameterizedInfo.delegate = delegate;
        job.parameterizedInfo.stack = stack;
        Detail::GetFramework()->Schedule(1, &job, lane);
    }

    /// Schedule helper
    /// \param info
    static void Schedule(Group &group, const Core::Delegate<void()> &delegate, const LaneID &lane = kDefault) {
        group.Increment();

        ScheduledJob job;
        job.isParameterized = false;
        job.info.delegate = delegate;
        job.group = group.GetGroup();
        job.post.argument = 1;
        job.post.op = CounterOp::eAdd;
        Detail::GetFramework()->Schedule(1, &job, lane);
    }

    /// Schedule helper
    /// \param info
    static void Schedule(Group &group, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack, const LaneID &lane = kDefault) {
        group.Increment();

        ScheduledJob job;
        job.isParameterized = true;
        job.parameterizedInfo.delegate = delegate;
        job.parameterizedInfo.stack = stack;
        job.group = group.GetGroup();
        job.post.argument = 1;
        job.post.op = CounterOp::eAdd;
        Detail::GetFramework()->Schedule(1, &job, lane);
    }

    /// Yield helper
    static void Yield() {
        Detail::GetFramework()->Yield();
    }

    /// Yield helper
    static void Yield(JobGroup *group) {
        Detail::GetFramework()->Yield(group);
    }

    /// SetCallGraph helper
    static void SetCallGraph(CallGraph *callGraph) {
        Detail::GetFramework()->SetCallGraph(callGraph);
    }

    /// Chunked helper
    template<typename F>
    void Chunked(Group &group, USize count, F &&action, const LaneID &lane = kDefault) {
        ScheduledJob jobs[128];

        // Approximate job count
        UInt32 jobCount = Math::Min(Detail::GetFramework()->GetScheduler()->GetWorkerCount(), static_cast<UInt32>(count));// + (UInt32)Math::Floor((count*0.0001)*8);
        if (jobCount == 0) {
            return;
        }

        DJINN_ASSERT(jobCount < 128, "Bad implementation");

        // Average action count
        USize actionCount = static_cast<USize>(Math::Floor(count / static_cast<double>(jobCount)));

        // ...
        group.Increment(jobCount);

        // Prepare jobs
        USize offset = 0;
        for (UInt32 i = 0; i < jobCount; i++) {
            // Determine total actions in this job
            USize jobActionCount = ((i == jobCount - 1) ? (count - offset) : actionCount);

            // End offset
            USize end = offset + jobActionCount;

            // Schedule info
            jobs[i].group = group.GetGroup();
            jobs[i].post.argument = 1;
            jobs[i].post.op = CounterOp::eAdd;
            jobs[i].isParameterized = true;
            jobs[i].parameterizedInfo.delegate = [=] {
                action(offset, end);
            };
            jobs[i].parameterizedInfo.delegate.SetRTTI(Core::DynamicDelegate(action));

            // ...
            offset += jobActionCount;
        }

        // Schedule
        Detail::GetFramework()->Schedule(jobCount, jobs, lane);
    }

    /// Foreach helper
    template<typename F>
    void Foreach(Group &group, USize count, F &&action, const LaneID &lane = kDefault) {
        Chunked(group, count, [=](USize begin, USize end) {
            for (USize j = begin; j < end; j++) {
                action(j);
            }
        });
    }

    /// Constructors
    namespace Detail {
        template<typename R>
        struct TaskScheduleCreator {
            template<typename... A>
            Task<R> Create(const Core::Delegate<R(A...)> &delegate, const LaneID &lane, const char* taskFrame, A... args) {
                // Allocate instance
                auto object = TaskObjectPool<R>::Instance().Pop();
                object->group.Increment();

                // Resulting task
                Task<R> task(object);

                // Prepare job
                ScheduledJob job;
                job.isParameterized = true;
                job.group = &object->group;
                job.post.argument = 1;
                job.post.op = CounterOp::eAdd;
                job.name = taskFrame;

                // Wrap delegate, the size of DynamicDelegate allows for a child Delegate<...>
                job.parameterizedInfo.stack = Core::Pack<Core::Delegate<R(A...)>, A...>(delegate, args...);
                job.parameterizedInfo.delegate = [=](Core::Delegate<R(A...)> _delegate, A... _args) {
                    new(object->Get()) R(_delegate.Invoke(_args...));
                };
                job.parameterizedInfo.delegate.SetRTTI(delegate);

                // Schedule task
                Detail::GetFramework()->Schedule(1, &job, lane);

                // ...
                return task;
            }

            template<typename... A>
            Task<R> CreateGrouped(const Core::Delegate<R(A...)> &delegate, Async::Group &group, const LaneID &lane, const char* taskFrame, A... args) {
                group.Increment();

                // Allocate instance
                auto object = TaskObjectPool<R>::Instance().Pop();

                // Resulting task
                Task<R> task(object, group.GetGroup());

                // Prepare job
                ScheduledJob job;
                job.isParameterized = true;
                job.group = group.GetGroup();
                job.post.argument = 1;
                job.post.op = CounterOp::eAdd;
                job.name = taskFrame;

                // Wrap delegate, the size of DynamicDelegate allows for a child Delegate<...>
                job.parameterizedInfo.stack = Core::Pack<Core::Delegate<R(A...)>, A...>(delegate, args...);
                job.parameterizedInfo.delegate = [=](Core::Delegate<R(A...)> _delegate, A... _args) {
                    new(object->Get()) R(_delegate.Invoke(_args...));
                };
                job.parameterizedInfo.delegate.SetRTTI(delegate);

                // Schedule task
                Detail::GetFramework()->Schedule(1, &job, lane);

                // ...
                return task;
            }
        };

        template<>
        struct TaskScheduleCreator<void> {
            template<typename... A>
            Task<void> Create(const Core::Delegate<void(A...)> &delegate, const LaneID &lane, const char* taskFrame, A... args) {
                // Allocate instance
                auto object = TaskObjectPool<void>::Instance().Pop();
                object->group.Increment();

                // Resulting task
                Task<void> task(object);

                // Prepare job
                ScheduledJob job;
                job.isParameterized = true;
                job.group = &object->group;
                job.post.argument = 1;
                job.post.op = CounterOp::eAdd;
                job.name = taskFrame;

                // Wrap delegate, the size of DynamicDelegate allows for a child Delegate<...>
                job.parameterizedInfo.stack = Core::Pack<A...>(args...);
                job.parameterizedInfo.delegate = delegate;
                job.parameterizedInfo.delegate.SetRTTI(delegate);

                // Schedule task
                Detail::GetFramework()->Schedule(1, &job, lane);

                // ...
                return task;
            }

            template<typename... A>
            Task<void> CreateGrouped(const Core::Delegate<void(A...)> &delegate, Async::Group &group, const LaneID &lane, const char* taskFrame, A... args) {
                group.Increment();

                // Allocate instance
                auto object = TaskObjectPool<void>::Instance().Pop();

                // Resulting task
                Task<void> task(object, group.GetGroup());

                // Prepare job
                ScheduledJob job;
                job.isParameterized = true;
                job.group = group.GetGroup();
                job.post.argument = 1;
                job.post.op = CounterOp::eAdd;
                job.name = taskFrame;

                // Wrap delegate, the size of DynamicDelegate allows for a child Delegate<...>
                job.parameterizedInfo.stack = Core::Pack<A...>(args...);
                job.parameterizedInfo.delegate = delegate;
                job.parameterizedInfo.delegate.SetRTTI(delegate);

                // Schedule task
                Detail::GetFramework()->Schedule(1, &job, lane);

                // ...
                return task;
            }
        };

        template<typename>
        struct TaskCreator;

        template<typename C>
        static const char* GetTaskFrame(C* frame) {
            if (frame) {
                if constexpr(Core::IsBaseOf<COM::IUnknown, C>::kValue) {
                    return frame->GetClassName();
                }
            }
            return nullptr;
        }

        template<typename C, typename R, typename... A>
        struct TaskCreator<R(C::*)(A...)> {
            Task<R> Create(const Core::Delegate<R(A...)> &delegate, const LaneID &lane, C* frame, A... args) {
                return TaskScheduleCreator<R>().template Create<A...>(delegate, lane, GetTaskFrame(frame), args...);
            }

            Task<R> CreateGrouped(const Core::Delegate<R(A...)> &delegate, Async::Group &group, const LaneID &lane, C* frame, A... args) {
                return TaskScheduleCreator<R>().template CreateGrouped<A...>(delegate, group, lane, GetTaskFrame(frame), args...);
            }
        };

        template<typename C, typename R, typename... A>
        struct TaskCreator<R(C::*)(A...) const> {
            Task<R> Create(const Core::Delegate<R(A...)> &delegate, const LaneID &lane, C* frame, A... args) {
                return TaskScheduleCreator<R>().template Create<A...>(delegate, lane, GetTaskFrame(frame), args...);
            }

            Task<R> CreateGrouped(const Core::Delegate<R(A...)> &delegate, Async::Group &group, const LaneID &lane, C* frame, A... args) {
                return TaskScheduleCreator<R>().template CreateGrouped<A...>(delegate, group, lane, GetTaskFrame(frame), args...);
            }
        };

        template<typename R, typename... A>
        struct TaskCreator<R(*)(A...)> {
            Task<R> Create(const Core::Delegate<R(A...)> &delegate, const LaneID &lane, void*, A... args) {
                return TaskScheduleCreator<R>().template Create<A...>(delegate, lane, nullptr, args...);
            }

            Task<R> CreateGrouped(const Core::Delegate<R(A...)> &delegate, Async::Group &group, const LaneID &lane, void*, A... args) {
                return TaskScheduleCreator<R>().template CreateGrouped<A...>(delegate, group, lane, nullptr, args...);
            }
        };
    }

/// Async helper function helper, member function
/// Fx. Asyncf(MemberFunction)
#define Asyncf(NAME) \
    template<typename... A> auto NAME##Async(A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&DJINN_QUALIFIED_THIS::NAME)>().Create(Bindff(NAME), Async::kDefault, this, args...); } \
    template<typename... A> auto NAME##Async(Async::Group& group, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&DJINN_QUALIFIED_THIS::NAME)>().CreateGrouped(Bindff(NAME), group, Async::kDefault, this, args...); } \
    template<typename... A> auto NAME##Async(Async::Group& group, const Async::LaneID& lane, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&DJINN_QUALIFIED_THIS::NAME)>().CreateGrouped(Bindff(NAME), group, lane, this, args...); } \
    template<typename... A> auto NAME##Async(const Async::LaneID& lane, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&DJINN_QUALIFIED_THIS::NAME)>().Create(Bindff(NAME), lane, this, args...); }

/// Async helper function helper, global function
/// Fx. Asyncf(GlobalFunction)
#define Asyncg(NAME) template<typename... A> auto NAME##Async(A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&NAME)>().Create(Bindg(NAME), Async::kDefault, nullptr, args...); } \
    template<typename... A> auto NAME##Async(Async::Group& group, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&NAME)>().CreateGrouped(Bindg(NAME), group, Async::kDefault, nullptr, args...); } \
    template<typename... A> auto NAME##Async(Async::Group& group, const Async::LaneID& lane, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&NAME)>().CreateGrouped(Bindg(NAME), group, lane, nullptr, args...); } \
    template<typename... A> auto NAME##Async(const Async::LaneID& lane, A&&... args) -> Async::Task<decltype(NAME(args...))> \
    { return Async::Detail::TaskCreator<decltype(&NAME)>().Create(Bindg(NAME), lane, nullptr, args...); }
}
