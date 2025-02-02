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
 on 7/11/2018.
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
