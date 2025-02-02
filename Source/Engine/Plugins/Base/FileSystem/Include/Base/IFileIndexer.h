//
// Created by Dev on 3/20/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "Paths.h"
#include "FileEvent.h"

namespace Djinn::Base {
    class IFileProxy;

    class IFileIndexer : public COM::TUnknown<IFileIndexer> {
    public:
        DJINN_COM_INTERFACE();

        IFileIndexer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };
}