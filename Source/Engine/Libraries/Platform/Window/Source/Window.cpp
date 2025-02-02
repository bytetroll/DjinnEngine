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
 on 12/11/2017.
//

#include <Platform/Window.h>

DJINN_NS2(Platform);

Window::Window() {

}

COM::Result Window::Initialize(const WindowDesc &desc) {
    this->desc = desc;

    // Result
    COM::Result result = COM::kOK;

    // Initialize backend window
    // Some platforms lock events to the current thread, so spawn a new thread
    thread.Start(Bindff(InitializeWindow), {&result});

    // Wait for initialization
    event.Wait();

    // ...
    return result;
}

float Window::GetAspectRatio() const {
    return static_cast<float>(desc.width) / static_cast<float>(desc.height);
}

COM::Result Window::PoolEvents(UInt32 &count, WindowEvent *out) {
    LockGuard guard(cachedEventMtx);
    if (out) {
        Core::Memory::Copy(out, cachedEvents.Ptr() + (cachedEvents.Size() - count), sizeof(WindowEvent) * count);
        cachedEvents.Remove(cachedEvents.Size() - count, cachedEvents.Size());
    } else {
        count = static_cast<UInt32>(cachedEvents.Size());
    }
    return COM::kOK;
}
