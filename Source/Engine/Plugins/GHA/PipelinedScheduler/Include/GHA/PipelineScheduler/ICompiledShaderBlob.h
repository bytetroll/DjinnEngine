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
 on 12/9/2017.
//
#pragma once

#include <COM/ID.h>
#include <Core/Array.h>
#include <GHA/ShaderBlob.h>
#include "DescriptorType.h"

namespace Djinn::GHA::PipelineScheduler {
    struct CompiledDescriptor {
        UInt32 set = 0;
        UInt32 index = 0;
        UInt32 count = 0;
        DescriptorType rid = DescriptorType::eNone;
        Core::String name;
    };

    class ICompiledShaderBlob : public GHA::ICompiledShaderBlob {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        ICompiledShaderBlob(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : GHA::ICompiledShaderBlob(registry, outer, host) {

        }

        // Resources
        Core::HashMap<UInt32, Core::Array<CompiledDescriptor>> resources;

        // Dynamic properties
        UInt32 dynamicOffset = 0;
        UInt32 dynamicSize = 0;

        // Dynamic bindings
        Core::HashMap<UInt64, UInt32> dynamicBindingOffsets;
    };
}