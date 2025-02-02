//
// Created by Dev on 12/9/2017.
//
#pragma once

#include <COM/ID.h>
#include <Core/Array.h>
#include <GHA/PipelineScheduler/ICompiledShaderBlob.h>

namespace Djinn::GHA::Vulkan {
    class CompiledShaderBlob : public PipelineScheduler::ICompiledShaderBlob {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        CompiledShaderBlob(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : PipelineScheduler::ICompiledShaderBlob(registry, outer, host) {

        }

        // ...
        Core::Array<UInt32> blob;
    };
}