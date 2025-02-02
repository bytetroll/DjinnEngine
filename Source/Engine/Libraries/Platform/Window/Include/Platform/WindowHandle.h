//
// Created by Dev on 12/11/2017.
//
#pragma once

#ifdef DJINN_PLATFORM_WINDOWS
class HWND__;
typedef HWND__ *HWND;
class HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;
#else
struct X11Display;
typedef unsigned long X11Window;
#endif

namespace Djinn::Platform {
    struct WindowHandle {
#ifdef DJINN_PLATFORM_WINDOWS
        HWND hWnd;
        HINSTANCE hInstance;
#else
        ::X11Display* display;
        ::X11Window window;
#endif
    };
}