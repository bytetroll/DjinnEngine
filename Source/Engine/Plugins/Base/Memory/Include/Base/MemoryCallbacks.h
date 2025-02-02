//
// Created by Dev on 5/13/2018.
//
#pragma once

#include <Platform/LibraryAPI.h>
#include <Core/Size.h>

namespace Djinn::Base {
    /// Callbacks
    DJINN_EXPORT_PLUGINS_BASE_MEMORY void *MallocCallback(void* user, USize size);
    DJINN_EXPORT_PLUGINS_BASE_MEMORY void FreeCallback(void* user, void *block);
}