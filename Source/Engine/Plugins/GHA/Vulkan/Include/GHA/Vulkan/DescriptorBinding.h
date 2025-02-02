#pragma once

#include <Core/Optional.h>
#include <Core/TypeAssignable.h>
#include <GHA/ID.h>
#include <GHA/PipelineScheduler/PipelineBinding.h>

namespace Djinn {
    namespace GHA {
        class IResource;
        class ISampler;

        namespace Vulkan {
            struct DescriptorBinding {
                DescriptorBinding &Resource(IResource *value) {
                    resource = value;
                    return *this;
                }

                DescriptorBinding &Sampler(ISampler *value) {
                    sampler = value;
                    return *this;
                }

                DescriptorBinding &Binding(PipelineScheduler::PipelineBinding value) {
                    binding = value;
                    return *this;
                }

                DescriptorBinding &ArrayElement(UInt32 value) {
                    arrayElement = value;
                    return *this;
                }

                USize Hash() {
                    return CombineHash(resource, binding.Hash(), arrayElement);
                }

                // Resource to bind
                IResource *resource = nullptr;

                // Sampler to bind
                ISampler *sampler = nullptr;

                // Pipeline binding
                PipelineScheduler::PipelineBinding binding;

                // Destination array element
                // Pipeline binding target must be of an array nature
                UInt32 arrayElement = 0;
            };
        }
    }
}