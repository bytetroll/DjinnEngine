//
// Created by Dev on 12/11/2017.
//

#include <X11/Xlib.h>

#undef Always

// Conventions are not ignored for macro expansions
#undef DefaultScreen
#undef ScreenOfDisplay
#define DefaultScreen(dpy) 	(reinterpret_cast<_XPrivDisplay>(dpy)->default_screen)
#define ScreenOfDisplay(dpy, scr) (&reinterpret_cast<_XPrivDisplay>(dpy)->screens[scr])

#include <Platform/Window.h>

DJINN_NS2(Platform);

#include <Core/Atomic.h>
#include <Platform/User.h>
#include <Platform/WindowEvent.h>

namespace {
    thread_local Platform::Window *self = nullptr;

    /*VirtualKeyCode SwitchVirtualKeyCode(WPARAM wparam) {
        switch (wparam) {
            case VK_LBUTTON:
                return VirtualKeyCode::eMouseLeft;
            case VK_RBUTTON:
                return VirtualKeyCode::eMouseRight;
            case VK_SCROLL:
                return VirtualKeyCode::eMouseScroll;
            case VK_BACK:
                return VirtualKeyCode::eBackspace;
            case VK_CLEAR:
                return VirtualKeyCode::eClear;
            case VK_RETURN:
                return VirtualKeyCode::eEnter;
            case VK_SHIFT:
                return VirtualKeyCode::eShift;
            case VK_CONTROL:
                return VirtualKeyCode::eCtrl;
            case VK_MENU:
                return VirtualKeyCode::eAlt;
            case VK_PAUSE:
                return VirtualKeyCode::ePause;
            case VK_CAPITAL:
                return VirtualKeyCode::eCapsLock;
            case VK_ESCAPE:
                return VirtualKeyCode::eEscape;
            case VK_SPACE:
                return VirtualKeyCode::eSpacebar;
            case VK_PRIOR:
                return VirtualKeyCode::ePageUp;
            case VK_NEXT:
                return VirtualKeyCode::ePageDown;
            case VK_HOME:
                return VirtualKeyCode::eHome;
            case VK_UP:
                return VirtualKeyCode::eArrowUp;
            case VK_DOWN:
                return VirtualKeyCode::eArrowDown;
            case VK_LEFT:
                return VirtualKeyCode::eArrowLeft;
            case VK_RIGHT:
                return VirtualKeyCode::eArrowRight;
            case VK_SELECT:
                return VirtualKeyCode::eSelect;
            case VK_PRINT:
                return VirtualKeyCode::ePrint;
            case VK_SNAPSHOT:
                return VirtualKeyCode::ePrintScreen;
            case VK_INSERT:
                return VirtualKeyCode::eInsert;
            case VK_DELETE:
                return VirtualKeyCode::eDelete;
            default:
                return VirtualKeyCode::eCharacter;
        }
    }*/
}

void Platform::Window::InitializeWindow(COM::Result *result) {
    *result = COM::kOK;

    // Assign self
    self = this;

    // ...
    auto &display = reinterpret_cast<Display *&>(handle.display);

    // Open display
    display = XOpenDisplay(nullptr);

    // Default visual
    auto visual = DefaultVisual(display, 0);

    // Default depth
    auto depth = DefaultDepth(display, 0);

    // Attributes
    XSetWindowAttributes attributes;
    attributes.background_pixel = XWhitePixel(display, 0);

    // Create window
    handle.window = XCreateWindow(
            display,
            XRootWindow(display, 0),
            0, 0,
            desc.width, desc.height,
            5, depth, InputOutput, visual, CWBackPixel, &attributes
    );

    // Set name
    XStoreName(display, handle.window, desc.title.AsCharArray());

    // Set input settings
    XSelectInput(display, handle.window, ExposureMask | StructureNotifyMask);

    // Map
    XMapWindow(display, handle.window);

    // Signal initialization event
    event.Signal();

    // Get messages
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, handle.window, &wmDeleteMessage, 1);

    // Listen
    for (bool _break = false; !_break;) {
        XEvent event;
        XNextEvent(display, &event);

        // Handle events
        switch (event.type) {
            case ClientMessage:
                if (event.xclient.data.l[0] == static_cast<UInt32>(wmDeleteMessage)) {
                    XDestroyWindow(display, handle.window);
                    _break = true;
                }
                break;
            default:
                break;
        }
    }

    // Close
    XCloseDisplay(display);

    // Signal finish event
    finish.Signal();
}

/*LRESULT Window::OnMessage(HWND hWnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    // Translate message
    switch (umessage) {
        case WM_CHAR: {
            switch (wparam) {
                case VK_BACK:
                case VK_RETURN:
                case VK_ESCAPE:
                    // Ignore values
                    break;
                default: {
                    WindowEvent event;
                    event.character = {};
                    event.character.keyCode = static_cast<KeyCode>(wparam);
                    {
                        LockGuard guard(cachedEventMtx);
                        cachedEvents.Add(event);
                    }
                    break;
                }
            }
            break;
        }
        case WM_KEYDOWN: {
            // Character event
            if (SwitchVirtualKeyCode(wparam) != VirtualKeyCode::eCharacter) {
                WindowEvent event;
                event.character = {};
                event.character.virtualKeyCode = SwitchVirtualKeyCode(wparam);
                event.character.keyCode = (event.keyDown.virtualKeyCode == VirtualKeyCode::eCharacter)
                                          ? static_cast<KeyCode>(wparam) : static_cast<KeyCode>(MapVirtualKey(
                                static_cast<UInt32>(wparam), MAPVK_VK_TO_CHAR));
                {
                    LockGuard guard(cachedEventMtx);
                    cachedEvents.Add(event);
                }
            }

            // 30	The previous key state. The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
            if (((lparam >> 30) & 0x1) == 0) {
                WindowEvent event;
                event.keyPressed = {};
                event.keyPressed.virtualKeyCode = SwitchVirtualKeyCode(wparam);
                event.keyPressed.keyCode = (event.keyPressed.virtualKeyCode == VirtualKeyCode::eCharacter)
                                           ? static_cast<KeyCode>(wparam) : static_cast<KeyCode>(MapVirtualKey(
                                static_cast<UInt32>(wparam),
                                MAPVK_VK_TO_CHAR));
                {
                    LockGuard guard(cachedEventMtx);
                    cachedEvents.Add(event);
                }
            } else {
                WindowEvent event;
                event.keyDown = {};
                event.keyDown.virtualKeyCode = SwitchVirtualKeyCode(wparam);
                event.keyDown.keyCode = (event.keyDown.virtualKeyCode == VirtualKeyCode::eCharacter)
                                        ? static_cast<KeyCode>(wparam) : static_cast<KeyCode>(MapVirtualKey(
                                static_cast<UInt32>(wparam), MAPVK_VK_TO_CHAR));
                {
                    LockGuard guard(cachedEventMtx);
                    cachedEvents.Add(event);
                }
            }
            break;
        }
        case WM_KEYUP: {
            WindowEvent event;
            event.keyReleased = {};
            event.keyReleased.virtualKeyCode = SwitchVirtualKeyCode(wparam);
            event.keyReleased.keyCode = (event.keyDown.virtualKeyCode == VirtualKeyCode::eCharacter)
                                        ? static_cast<KeyCode>(wparam) : static_cast<KeyCode>(MapVirtualKey(
                            static_cast<UInt32>(wparam), MAPVK_VK_TO_CHAR));
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            WindowEvent event;
            event.mousePressed = {};
            event.mousePressed.virtualKeyCode = VirtualKeyCode::eMouseLeft;
            event.mousePressed.position = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_LBUTTONUP: {
            WindowEvent event;
            event.mouseReleased = {};
            event.mouseReleased.virtualKeyCode = VirtualKeyCode::eMouseLeft;
            event.mouseReleased.position = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_RBUTTONDOWN: {
            WindowEvent event;
            event.mousePressed = {};
            event.mousePressed.virtualKeyCode = VirtualKeyCode::eMouseRight;
            event.mousePressed.position = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_RBUTTONUP: {
            WindowEvent event;
            event.mouseReleased = {};
            event.mouseReleased.virtualKeyCode = VirtualKeyCode::eMouseRight;
            event.mouseReleased.position = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_MOUSEMOVE: {
            WindowEvent event;
            event.mouseMoved = {};
            event.mouseMoved.position = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            break;
        }
        case WM_CLOSE: {
            PostQuitMessage(0);

            WindowEvent event;
            event.type = WindowEventType::eClosed;
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);

            WindowEvent event;
            event.type = WindowEventType::eClosed;
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }
            return 0;
        }
        default: {
            return DefWindowProc(hWnd, umessage, wparam, lparam);
        }
    }

    // OK
    return 0;
}*/

void Platform::Window::Show(bool isVisible) {
    //ShowWindow(handle.hWnd, isVisible ? SW_SHOW : SW_HIDE);
}

void Platform::Window::SetFocus() {
    //::SetFocus(handle.hWnd);
}

void Platform::Window::SetForegroundWindow() {
    //::SetForegroundWindow(handle.hWnd);
}

void Platform::Window::Minimize() {
    //::ShowWindow(handle.hWnd, SW_MINIMIZE);
}

void Platform::Window::Maximize() {
    //::ShowWindow(handle.hWnd, SW_MAXIMIZE);
}

void Platform::Window::Close() {
    //::CloseWindow(handle.hWnd);
}

void Platform::Window::Wait() {
    finish.Wait();
}

void Platform::Window::SetLocalMousePosition(const Djinn::Math::Vector2I &position) {
    XWarpPointer(
            reinterpret_cast<Display *>(handle.display),
            0,
            handle.window,
            0, 0,
            0, 0,
            position.x, position.y
    );
}

Math::Vector2I Platform::Window::GetSize() {
    Int32 _dummy[2];
    UInt32 width, height, bwidth, depth;

    // ...
    auto display = reinterpret_cast<Display *>(handle.display);

    // Query
    auto root = XRootWindow(display, 0);
    //XGetGeometry(display, handle.window, &root, &_dummy[0], &_dummy[1], &width, &height, &bwidth, &depth);

    return Math::Vector2I(640, 480);

    // ...
    //return {width, height};
}

Math::Vector2I Platform::Window::GetPosition() {
    Int32 _dummy[2];
    UInt32 width, height, bwidth, depth;

    // ...
    auto display = reinterpret_cast<Display *>(handle.display);

    // Query
    auto root = XRootWindow(display, 0);
    //XGetGeometry(display, handle.window, &root, &_dummy[0], &_dummy[1], &width, &height, &bwidth, &depth);

    return Math::Vector2I(640, 480);

    // ...
    //return {width, height};
}
