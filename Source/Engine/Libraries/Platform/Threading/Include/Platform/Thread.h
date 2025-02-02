//0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0
//>>
//>>
//>>                ████████▄       ▄█  ▄█  ███▄▄▄▄   ███▄▄▄▄
//>>                ███   ▀███     ███ ███  ███▀▀▀██▄ ███▀▀▀██▄
//>>                ███    ███     ███ ███▌ ███   ███ ███   ███
//>>                ███    ███     ███ ███▌ ███   ███ ███   ███
//>>                ███    ███     ███ ███▌ ███   ███ ███   ███
//>>                ███    ███     ███ ███  ███   ███ ███   ███
//>>                ███   ▄███     ███ ███  ███   ███ ███   ███
//>>                ████████▀  █▄ ▄███ █▀    ▀█   █▀   ▀█   █▀
//>>                ▀▀▀▀▀▀
//>>
//>>                            ------------------------
//>>                               Copyright (C) MMXVII
//>>                            ------------------------
//>>                               Miguel N. Petersen
//>>                                 Nathan C. Young
//>>
//0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0
//>>
//0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0
#pragma once

#include "Mutex.h"
#include "LockGuard.h"
#include <Core/DynamicDelegate.h>
#include <Core/String.h>
#include <Platform/ThreadAPI.h>

namespace Djinn {
    namespace Platform {
        enum class ThreadState {
            eIdle, eRunning
        };

        class Thread {
        public:
            DJINN_NOCOPY(Thread);

            Thread( );

            /// Start this thread with given callbacks
            /// \param function
            /// \param stack
            void Start( const Core::DynamicDelegate& function, const Core::DynamicParameterStack& stack = {} );

            /// Returns true if the thread can be joined
            /// \return
            bool CanJoin( );

            /// Join this thread, will block until completion
            /// \return
            bool Join( );

            /// Detach from this thread
            void Detach( );

            /// Get OS dependent id
            /// \return
            ThreadAPI::ThreadID GetID( );

            /// Get current state of this thread
            /// \return
            ThreadState GetState();

            /// Set os name of this thread
            /// \param name
            void SetName(const Core::String& name);

        protected:
            /// Internal thread data
            struct ThreadData {
                Thread* instance;
                Core::DynamicDelegate function;
                Core::DynamicParameterStack stack;
            };

            /// Callback wrapper
            /// \param data
            static void Callback( void* data );

        private:
            /// Current state of this thread
            ThreadState state;

            /// Supplied name
            Core::String name;

            /// Data mutex
            Mutex dataMutex;

            /// API Handle
            ThreadAPI::Handle handle;
        };
    }
}