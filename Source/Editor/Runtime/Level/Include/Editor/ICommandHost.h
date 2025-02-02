//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "CommandFlag.h"

namespace Djinn::Editor {
    struct Command;

    /// Common results
    DJINN_COM_RESULT(NoCommand, false, "Editor");
    DJINN_COM_RESULT(NoCommandAction, true, "Editor");

    class ICommandHost : public COM::TUnknown<ICommandHost> {
    public:
        DJINN_COM_INTERFACE();

        ICommandHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Queue a command for execution
        /// \param command
        virtual void Queue(const Command& command) = 0;

        /// Process all queued commands
        virtual void Process() = 0;

        /// Undo a command
        /// \return
        virtual COM::Result Undo(const CommandFlagSet& flag = CommandFlag::eNone) = 0;

        /// Redo a command
        /// \return
        virtual COM::Result Redo(const CommandFlagSet& flag = CommandFlag::eNone) = 0;
    };
}