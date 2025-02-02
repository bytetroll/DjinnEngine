//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 12/13/2017.
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