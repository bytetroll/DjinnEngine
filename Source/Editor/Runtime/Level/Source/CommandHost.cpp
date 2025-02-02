//
// Created by Dev on 7/11/2018.
//

#include <Editor/CommandHost.h>

DJINN_NS2(Editor);

CommandHost::CommandHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ICommandHost(registry, outer, host) {

}

void CommandHost::Queue(const Command &command) {
    buffer.Add(command);
}

void CommandHost::Process() {
    // Reset index
    history.Resize(historyHead + 1);

    // Process commands
    for (auto &cmd : buffer) {
        // Invoke
        if (cmd.action) {
            cmd.action();
        }

        // Submit to history
        history.Add(cmd);
        historyHead++;
    }

    // ...
    buffer.Clear();

}

COM::Result CommandHost::Undo(const CommandFlagSet &flag) {
    if (history.Size() <= historyHead) {
        return kNoCommand;
    }

    // Get command
    auto &cmd = history[historyHead - 1];

    // Not undoable?
    if (!cmd.undo && !(flag & CommandFlag::eSkipIfFail)) {
        return kNoCommandAction;
    }

    // Invoke if needed
    if (cmd.undo && !(flag & CommandFlag::eSkip)) {
        cmd.undo();
    }

    // ...
    historyHead--;
    return COM::kOK;
}

COM::Result CommandHost::Redo(const CommandFlagSet &flag) {
    if (history.Size() <= historyHead) {
        return kNoCommand;
    }

    // Get command
    auto &cmd = history[historyHead - 1];

    // Not redoable?
    if (!cmd.action && !(flag & CommandFlag::eSkipIfFail)) {
        return kNoCommandAction;
    }

    // Invoke if needed
    if (cmd.action && !(flag & CommandFlag::eSkip)) {
        cmd.action();
    }

    // ...
    historyHead++;
    return COM::kOK;
}
