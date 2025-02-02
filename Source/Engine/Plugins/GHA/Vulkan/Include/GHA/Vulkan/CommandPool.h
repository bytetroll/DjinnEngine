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
 on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/PipelineScheduler/CommandPoolDesc.h>
#include <GHA/PipelineScheduler/CommandListDesc.h>

namespace Djinn::GHA::Vulkan {
    class Queue;
    class CommandList;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN CommandPool : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        CommandPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const PipelineScheduler::CommandPoolDesc &desc, Queue *queue, vk::Device device);

        /// Overrides
        const PipelineScheduler::CommandPoolDesc &GetDesc();
        COM::Result CreateCommandList(COM::IUnknown *outer, const PipelineScheduler::CommandListDesc &desc, CommandList **out);

        /// Get pool
        /// \return
        vk::CommandPool &GetPool() {
            return pool;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        Queue *queue;
        PipelineScheduler::CommandPoolDesc desc;
        vk::Device device;
        vk::CommandPool pool;
    };
}