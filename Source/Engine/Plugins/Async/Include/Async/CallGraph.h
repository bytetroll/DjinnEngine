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
 on 3/14/2018.
//
#pragma once

#include <Core/Array.h>
#include <Core/Delegate.h>
#include <Core/SparseArray.h>
#include <Platform/SpinMutex.h>
#include <Platform/Thread.h>
#include <Core/Buffer.h>

namespace Djinn::Async {
    struct CallHeader {
        /// Check if this header is valid
        /// \return
        operator bool() const {
            return uid != -1;
        }

        USize index = 0;
        Int64 uid = -1;
    };

    /// Represents a single call
    struct RecordedCall {
        /// Parent caller
        /// -1: No parent
        CallHeader parent;

        /// Runtime type information
        Core::DelegateRTTI rtti;

        /// Name of the job
        const char* name = nullptr;

        /// Actual elapsed time of this call
        double runningTime = 0;

        /// Start time stamp of this call
        double startTimeStamp = 0;

        /// End time stamp of this call
        double endTimeStamp = 0;

        /// Previous yielder
        CallHeader lastYield;

        /// UID of this call
        Int64 uid = 0;

        /// Thread id
        Platform::ThreadAPI::ThreadID threadID;
    };

    class CallGraph {
    public:
        /// Add a call asynchronously
        /// \param call
        /// \return
        CallHeader Add(const RecordedCall& call) {
            calls.Add(call);
            calls.Last().uid = uid;
            return {calls.Size() - 1, uid++};
        }

        /// Check if this call graph contains a call
        /// \param index
        /// \param functor
        bool Contains(CallHeader header) {
            return header.index < calls.Size() && calls[header.index].uid == header.uid;
        }

        /// Get a call
        /// \param index
        /// \return
        const RecordedCall& Get(CallHeader header) const {
            return calls[header.index];
        }

        /// Get a call
        /// \param index
        /// \return
        const RecordedCall& Get(USize index) const {
            return calls[index];
        }

        /// Get a call
        /// \param index
        /// \return
        RecordedCall& Get(CallHeader header) {
            return calls[header.index];
        }

        /// Get the call count
        /// \return
        USize GetSize() const {
            return calls.Size();
        }

        /// Flush this call graph
        void Flush() {
            calls.Clear();
        }

        /// Mark the state
        /// \param state
        void SetRecording(bool state) {
            isRecording = state;
        }

        /// Is this graph in a recording state?
        /// \return
        bool IsRecording() const {
            return isRecording;
        }

        /// Lock this graph
        void Lock() {
            mutex.Lock();
        }

        /// Unlock this graph
        void Unlock() {
            mutex.Unlock();
        }

    private:
        Platform::SpinMutex mutex;
        Core::Buffer<RecordedCall> calls;
        Int64 uid = 0;
        bool isRecording = false;
    };
}