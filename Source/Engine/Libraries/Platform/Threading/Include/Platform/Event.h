//
// Created by Dev on 1/12/2018.
//
#pragma once

#include "LockGuard.h"
#include "ConditionalVar.h"

namespace Djinn::Platform {
    class Event {
    public:
        /// Wait for an event and do not reset when signalled
        void WaitRepeat() {
            var.Wait(mutex, [&] { return flag; });
        }

        /// Wait for an event
        void Wait() {
            var.Wait(mutex, [&] { return flag; });
            flag = false;
        }

        /// Signal all waiting events to wake
        void Signal() {
            {
                LockGuard guard(mutex);
                flag = true;
            }
            var.SignalAll();
        }

    private:
        Mutex mutex;
        bool flag = false;
        ConditionalVar var;
    };
}