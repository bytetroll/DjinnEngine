#pragma once


#include "CompareOp.h"

namespace Djinn {
    namespace GHA {
        struct DepthStencilStateDesc {
            DepthStencilStateDesc& MinDepth(float value) {
                minDepth = value;
                return *this;
            }

            DepthStencilStateDesc& MaxDepth(float value) {
                maxDepth = value;
                return *this;
            }

            DepthStencilStateDesc& DepthRead(bool value) {
                depthRead = value;
                return *this;
            }

            DepthStencilStateDesc& DepthWrite(bool value) {
                depthWrite = value;
                return *this;
            }

            DepthStencilStateDesc& StencilRead(bool value) {
                stencilRead = value;
                return *this;
            }

            DepthStencilStateDesc& DepthOp(CompareOp value) {
                depthOp = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(minDepth, maxDepth, depthRead, depthWrite, stencilRead, depthOp);
            }

            // Min depth
            float minDepth = 0.0f;

            // Max depth
            float maxDepth = 1.0f;

            // Perform a depth test?
            bool depthRead = true;

            // Perform a depth write?
            bool depthWrite = true;

            // Perform a stencil test?
            bool stencilRead = false;

            // Depth comparison
            CompareOp depthOp = CompareOp::eLessOrEqual;
        };
    }
}