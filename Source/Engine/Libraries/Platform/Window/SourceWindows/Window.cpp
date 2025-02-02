//
// Created by Dev on 12/11/2017.
//

#include <Platform/Window.h>

DJINN_NS2(Platform);

#include <tchar.h>
#include <Core/Windows.h>
#include <Core/Atomic.h>
#include <Platform/User.h>
#include <Platform/WindowEvent.h>

#include <Windowsx.h>

namespace {
    Core::AtomicCounter classCounter{0};
    thread_local Window *self = nullptr;

    VirtualKeyCode SwitchVirtualKeyCode(WPARAM wparam) {
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
    }

    KeyCode SwitchKeyCode(WPARAM code) {
        switch (code) {
            case 'A':
                return KeyCode::eA;
            case 'B':
                return KeyCode::eB;
            case 'C':
                return KeyCode::eC;
            case 'D':
                return KeyCode::eD;
            case 'E':
                return KeyCode::eE;
            case 'F':
                return KeyCode::eF;
            case 'G':
                return KeyCode::eG;
            case 'H':
                return KeyCode::eH;
            case 'I':
                return KeyCode::eI;
            case 'J':
                return KeyCode::eJ;
            case 'K':
                return KeyCode::eK;
            case 'L':
                return KeyCode::eL;
            case 'M':
                return KeyCode::eM;
            case 'N':
                return KeyCode::eN;
            case 'O':
                return KeyCode::eO;
            case 'P':
                return KeyCode::eP;
            case 'Q':
                return KeyCode::eQ;
            case 'R':
                return KeyCode::eR;
            case 'S':
                return KeyCode::eS;
            case 'T':
                return KeyCode::eT;
            case 'U':
                return KeyCode::eU;
            case 'V':
                return KeyCode::eV;
            case 'W':
                return KeyCode::eW;
            case 'X':
                return KeyCode::eX;
            case 'Y':
                return KeyCode::eY;
            case 'Z':
                return KeyCode::eZ;
            default:
                return KeyCode::eNone;
        }
    }
}

static WINAPI LRESULT WindowProcess(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    return self->OnMessage(hwnd, umessage, wparam, lparam);
}

void Window::InitializeWindow(COM::Result *result) {
    *result = COM::kOK;

    // Assign self
    self = this;

    // Get instance
    handle.hInstance = GetModuleHandle(nullptr);

    // Class name
    Core::String className = desc.title + classCounter++;

    // Describe window class
    WNDCLASSEX _class = {};
    _class.cbSize = sizeof(WNDCLASSEX);
    _class.cbClsExtra = 0;
    _class.cbWndExtra = 0;
    _class.hInstance = handle.hInstance;
    _class.hIcon = 0;
    _class.hIconSm = _class.hIcon;
    _class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    _class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    _class.lpszMenuName = nullptr;
    _class.lpszClassName = className.AsCharArray();
    _class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    _class.lpfnWndProc = WindowProcess;

    // Register class
    RegisterClassEx(&_class);

    // Create window
    handle.hWnd = CreateWindowEx(
            WS_EX_APPWINDOW,
            className.AsCharArray(),
            desc.title.AsCharArray(),
            WS_OVERLAPPEDWINDOW,
            (User::GetPrimaryDisplayWidth() / 2) - (desc.width / 2),
            (User::GetPrimaryDisplayHeight() / 2) - (desc.height / 2),
            desc.width,
            desc.height,
            nullptr,
            nullptr,
            handle.hInstance,
            0
    );

    // Signal initialization event
    event.Signal();

    // Listen
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    for (bool _break = false; !_break;) {
        // Wait for message
        WaitMessage();

        // TODO: Figure out why manually specifying the hwnd handle ignores some messages such as WM_QUIT, odd issue
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                _break = true;
            }

            // Handle
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Signal finish event
    finish.Signal();
}

LRESULT Window::OnMessage(HWND hWnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
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
                    event.character.virtualKeyCode = VirtualKeyCode::eCharacter;
                    event.character.key = static_cast<char>(wparam);
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
                event.character.key = (event.keyDown.key.virtualKeyCode == VirtualKeyCode::eCharacter) ? static_cast<char>(wparam) : static_cast<char>(MapVirtualKey(
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
                event.keyPressed.key.virtualKeyCode = SwitchVirtualKeyCode(wparam);
                event.keyPressed.key.keyCode = (event.keyPressed.key.virtualKeyCode == VirtualKeyCode::eCharacter) ? SwitchKeyCode(wparam) : SwitchKeyCode(MapVirtualKey(
                        static_cast<UInt32>(wparam),
                        MAPVK_VK_TO_CHAR));
                {
                    LockGuard guard(cachedEventMtx);
                    cachedEvents.Add(event);
                }
            } else {
                WindowEvent event;
                event.keyDown = {};
                event.keyDown.key.virtualKeyCode = SwitchVirtualKeyCode(wparam);
                event.keyDown.key.keyCode = (event.keyDown.key.virtualKeyCode == VirtualKeyCode::eCharacter) ? SwitchKeyCode(wparam) : SwitchKeyCode(MapVirtualKey(
                        static_cast<UInt32>(wparam),
                        MAPVK_VK_TO_CHAR));
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
            event.keyReleased.key.virtualKeyCode = SwitchVirtualKeyCode(wparam);
            event.keyReleased.key.keyCode = (event.keyDown.key.virtualKeyCode == VirtualKeyCode::eCharacter) ? SwitchKeyCode(wparam) : SwitchKeyCode(MapVirtualKey(
                    static_cast<UInt32>(wparam),
                    MAPVK_VK_TO_CHAR));
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
            event.mouseMoved.delta = event.mouseMoved.position - mousePosition;
            {
                LockGuard guard(cachedEventMtx);
                cachedEvents.Add(event);
            }

            mousePosition = event.mouseMoved.position;
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
}

void Window::Show(bool isVisible) {
    ::ShowWindow(handle.hWnd, isVisible ? SW_SHOW : SW_HIDE);
}

void Window::SetFocus() {
    ::SetFocus(handle.hWnd);
}

void Window::SetForegroundWindow() {
    ::SetForegroundWindow(handle.hWnd);
}

void Window::Minimize() {
    ::ShowWindow(handle.hWnd, SW_MINIMIZE);
}

void Window::Maximize() {
    ::ShowWindow(handle.hWnd, SW_MAXIMIZE);
}

void Window::Close() {
    ::CloseWindow(handle.hWnd);
}

void Window::Wait() {
    finish.Wait();
}

void Window::SetLocalMousePosition(const Djinn::Math::Vector2I &position) {
    RECT rect;
    if (!GetClientRect(handle.hWnd, &rect)) {
        return;
    }

    POINT point;
    point.x = position.x;
    point.y = position.y;

    if (!ClientToScreen (handle.hWnd, &point)) {
        return;
    }

    SetCursorPos(point.x, point.y);
}

Math::Vector2I Window::GetSize() {
    RECT rect;
    if (GetWindowRect(handle.hWnd, &rect)) {
        return Math::Vector2I(rect.right - rect.left, rect.bottom - rect.top);
    }
    return {};
}

Math::Vector2I Window::GetPosition() {
    RECT rect;
    if (GetWindowRect(handle.hWnd, &rect)) {
        return Math::Vector2I(rect.left, rect.top);
    }
    return {};
}
