#pragma once

#include <Core/Optional.h>
#include <Core/TypeAssignable.h>
#include <GHA/ID.h>
#include "PipelineBinding.h"
#include "DescriptorType.h"

namespace Djinn {
    namespace GHA {
        namespace PipelineScheduler {
            struct DescriptorBinding {
                DescriptorBinding &Resource(GHA::ResourceID value) {
                    resource = value;
                    return *this;
                }

                DescriptorBinding &Sampler(GHA::SamplerID value) {
                    sampler = value;
                    return *this;
                }

                DescriptorBinding &Binding(PipelineBinding value) {
                    binding = value;
                    return *this;
                }

                DescriptorBinding &ArrayElement(UInt32 value) {
                    arrayElement = value;
                    return *this;
                }

                DescriptorBinding &Class(DescriptorType value) {
                    id = value;
                    return *this;
                }

                // Class of resource, may be none
                DescriptorType id = DescriptorType::eNone;

                // Resource to bind
                GHA::ResourceID resource;

                // Sampler to bind
                GHA::SamplerID sampler;

                // Pipeline binding
                PipelineBinding binding;

                // Destination array element
                // Pipeline binding target must be of an array nature
                UInt32 arrayElement = 0;
            };
        }
    }
}