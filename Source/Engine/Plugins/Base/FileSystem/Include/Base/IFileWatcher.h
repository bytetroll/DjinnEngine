//
// Created by Dev on 3/20/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "Paths.h"
#include "FileEvent.h"

namespace Djinn::Base {
    class IFileWatcher : public COM::TUnknown<IFileWatcher> {
    public:
        DJINN_COM_INTERFACE();

        IFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Called when an event is fired
        /// \param event
        /// \param path
        virtual void OnEvent(FileEvent event, const PathID& path) = 0;
    };
}