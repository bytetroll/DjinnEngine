//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <Core/Array.h>
#include "ICommandHost.h"
#include "Command.h"

namespace Djinn::Editor {
    class DJINN_EXPORT_EDITOR_LEVEL CommandHost : public ICommandHost {
    public:
        DJINN_COM_CLASS();

        CommandHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void Queue(const Command &command) override;
        void Process() override;
        COM::Result Undo(const CommandFlagSet& flag) override;
        COM::Result Redo(const CommandFlagSet& flag) override;

    private:
        Core::Array<Command> buffer;

        USize historyHead = 0;
        Core::Array<Command> history;
    };
}