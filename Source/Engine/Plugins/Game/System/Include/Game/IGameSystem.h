//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Base/IApplicationSystem.h>

namespace Djinn::Game {
    class IStepSystem;

    class DJINN_EXPORT_PLUGINS_GAME_SYSTEM IGameSystem : public Base::IApplicationSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IGameSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IApplicationSystem(registry, outer, host) {

        }

        /// Register a system
        /// \param system
        /// \return
        virtual COM::Result Register(IStepSystem *system) = 0;

        /// Deregister a system
        /// \param system
        /// \return
        virtual COM::Result Deregister(IStepSystem *system) = 0;

        /// Enable or disable the recording of async calls
        /// \param b
        virtual void EnableAsyncRecording(bool b = true) = 0;

        /// Get the async call graph of the last iteration
        /// \return
        virtual Async::CallGraph* GetAsyncGraph() = 0;

        /// Set maximum iterations per second
        /// \param ips
        virtual void SetMaxIPS(UInt32 ips) = 0;

        /// Get maximum iterations per second
        /// \return
        virtual UInt32 GetMaxIPS() = 0;

        /// Get iteration delta
        /// \return
        virtual double GetDelta() = 0;

        /// Flag for exit
        virtual void FlagExit() = 0;
    };
}