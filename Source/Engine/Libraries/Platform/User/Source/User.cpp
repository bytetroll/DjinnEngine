// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <Platform/User.h>
#include <Core/Common.h>

#ifdef DJINN_PLATFORM_WINDOWS
#include <Core/Windows.h>
#endif

#ifdef DJINN_PLATFORM_LINUX
    #include <unistd.h>
    #include <X11/Xlib.h>

    // Conventions are not ignored for macro expansions
#   undef DefaultScreen
#   undef ScreenOfDisplay
#   define DefaultScreen(dpy) 	(reinterpret_cast<_XPrivDisplay>(dpy)->default_screen)
#   define ScreenOfDisplay(dpy, scr) (&reinterpret_cast<_XPrivDisplay>(dpy)->screens[scr])
#endif

DJINN_NS2( Platform );

UInt32 User::GetCoreCount() {
    #ifdef DJINN_PLATFORM_WINDOWS
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        return sysInfo.dwNumberOfProcessors;

    #elif defined( DJINN_PLATFORM_LINUX )
        return static_cast< Djinn::UInt32 >( sysconf( _SC_NPROCESSORS_ONLN ) );
    #else
        #error Not implemented.
    #endif
}

UInt32 User::GetPrimaryDisplayWidth() {
    #ifdef DJINN_PLATFORM_WINDOWS
        return static_cast< Djinn::UInt32 >( GetSystemMetrics( SM_CXSCREEN ) );
    #elif defined( DJINN_PLATFORM_LINUX )
        // This will get the default, primary monitor attached and being used by
        // the x-org server.  A proper system will need to be put in place for
        // detecting multi-monitor setups - NCY.
        Display* const AttachedMonitor = XOpenDisplay( nullptr );
        Screen* const AttachedScreen = DefaultScreenOfDisplay( AttachedMonitor );

        return static_cast< Djinn::UInt32 >( AttachedScreen->width );
    #else
        #error Not implemented
    #endif
}

UInt32 User::GetPrimaryDisplayHeight() {
    #ifdef DJINN_PLATFORM_WINDOWS
        return static_cast< Djinn::UInt32 >( GetSystemMetrics( SM_CYSCREEN ) );
    #elif defined( DJINN_PLATFORM_LINUX )
        // See note above - NCY.
        Display* const AttachedMonitor = XOpenDisplay( nullptr );
        Screen* const AttachedScreen = DefaultScreenOfDisplay( AttachedMonitor );

        return static_cast< Djinn::UInt32 >( AttachedScreen->height );
    #else
        #error Not implemented
    #endif
}
