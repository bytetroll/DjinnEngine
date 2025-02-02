#pragma once

#include <Core/Types.h>
#include <Core/Size.h>

namespace Djinn {
    namespace GHA {
        namespace PipelineScheduler {
            struct PipelineBinding {
                PipelineBinding() {}

                PipelineBinding(UInt32 set, UInt32 index) : set(set), index(index) {

                }

                USize Hash() {
                    return CombineHash(set, index);
                }

                // Set this bind points to
                UInt32 set;

                // Index this bind points to
                UInt32 index;
            };
        }
    }
}