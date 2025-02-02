//
// Created by Dev on 3/19/2018.
//
#pragma once

#ifdef DJINN_PLATFORM_WINDOWS
#   define NOMINMAX
#   include <windows.h>
#   undef Yield
#   undef GetClassName
#   undef interface
#   undef UpdateResource
#endif