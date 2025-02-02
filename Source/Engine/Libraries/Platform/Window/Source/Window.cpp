//
// Created by Dev on 12/11/2017.
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
