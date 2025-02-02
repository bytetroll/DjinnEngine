#pragma once

#include <Core/Types.h>
#include <Core/Size.h>
#include <Core/BitSet.h>
#include <Core/TypeAssignable.h>
#include "SampleMode.h"
#include "RenderTargetOp.h"
#include "CullMode.h"
#include "FillMode.h"

namespace Djinn {
    namespace GHA {
        struct RasterizerStateDesc {
            RasterizerStateDesc() = default;

            RasterizerStateDesc(GHA::FillMode fillMode, GHA::CullMode cullMode, bool isCounterClockwise, float lineWidth) : fillMode(fillMode), cullMode(cullMode), isCounterClockwise(isCounterClockwise),
                                                                                                                            lineWidth(lineWidth) {
                // ...
            }

            RasterizerStateDesc &FillMode(GHA::FillMode value) {
                fillMode = value;
                return *this;
            }

            RasterizerStateDesc &CullMode(GHA::CullMode value) {
                cullMode = value;
                return *this;
            }

            RasterizerStateDesc &IsCounterClockwise(bool value) {
                isCounterClockwise = value;
                return *this;
            }

            RasterizerStateDesc &LineWidth(float value) {
                lineWidth = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(fillMode, cullMode, isCounterClockwise, lineWidth);
            }

            // Fill mode
            GHA::FillMode fillMode = GHA::FillMode::eFill;

            // Cull mode
            GHA::CullMode cullMode = GHA::CullMode::eBack;

            // Winding order
            bool isCounterClockwise = false;

            // Line width
            float lineWidth = 1.0f;
        };

        namespace Rasterizers {
            static auto noCull = RasterizerStateDesc(
                    FillMode::eFill, CullMode::eNone, false, 1.0f
            );
        }
    }
}