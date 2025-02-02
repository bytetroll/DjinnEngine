//
// Created by Dev on 3/14/2018.
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