//
// Created by Dev on 12/13/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/Optional.h>
#include <Core/Pair.h>
#include <Core/Atomic.h>
#include <Core/FixedArray.h>
#include "MemoryBounds.h"

namespace Djinn::GHA {
    class IResource;
    class IMemoryResource;
    class ITexture;

    class IResourceStager : public COM::TUnknown<IResourceStager> {
    public:
        DJINN_COM_INTERFACE();

        IResourceStager(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Stage a resource in bounds with buffer
        /// \param resource
        /// \param buffer - Lifetime must extend the call
        /// \param bounds
        /// \param signal - Set to true once the buffer has been uploaded, buffer can safely be released
        virtual void StageBuffer(IMemoryResource *resource, void *buffer, const GHA::MemoryBounds &bounds) = 0;

        /// Stage a texture with given mip buffers
        /// \param resource
        /// \param buffer - Lifetime must extend the call
        /// \param bounds
        /// \param signal - Set to true once the buffer has been uploaded, buffer can safely be released
        virtual void StageTextureMip(ITexture *texture, UInt32 mip, void *buffer, const GHA::MemoryBounds &bounds) = 0;

        /// Wait for all stages to complete
        virtual void Wait() = 0;
    };
}