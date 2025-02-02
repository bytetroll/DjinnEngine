//
// Created by Dev on 12/11/2017.
//

#include <Platform/Window.h>
#include <COM/ID.h>

DJINN_NS2(Platform);

int main() {
    // Create window
    Window window;
    Always(window.Initialize(WindowDesc().Title("Window Test")));

    window.Show();
    window.SetForegroundWindow();
    window.SetFocus();
    window.Wait();
}