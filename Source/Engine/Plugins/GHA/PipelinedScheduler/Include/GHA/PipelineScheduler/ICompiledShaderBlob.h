//
// Created by Dev on 12/9/2017.
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