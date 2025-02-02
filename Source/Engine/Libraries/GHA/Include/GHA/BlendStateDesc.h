#pragma once

#include <Core/Types.h>
#include <Core/FixedArray.h>
#include "BlendFactor.h"
#include "BlendOp.h"

namespace Djinn {
    namespace GHA {
        enum class Component {
            eR = 1,
            eG = static_cast<int>(eR) << 1,
            eB = static_cast<int>(eG) << 1,
            eA = static_cast<int>(eB) << 1,
            eAll = ~0
        };

        // Bitset
        DJINN_CORE_BITSET(Component);

        struct RenderTargetBlendDesc {
            RenderTargetBlendDesc() = default;

            RenderTargetBlendDesc(
                    bool enabled,
                    BlendOp colorOp, BlendFactor colorSrcFactor, BlendFactor colorDstFactor,
                    BlendOp alphaOp, BlendFactor alphaSrcFactor, BlendFactor alphaDstFactor,
                    ComponentSet mask
            ) : enabled(enabled), colorOp(colorOp), colorSrcFactor(colorSrcFactor), colorDstFactor(colorDstFactor),
                alphaOp(alphaOp), alphaSrcFactor(alphaSrcFactor), alphaDstFactor(alphaDstFactor), mask(mask) {

            }

            RenderTargetBlendDesc &Enabled(bool value) {
                enabled = value;
                return *this;
            }

            RenderTargetBlendDesc &ColorOp(BlendOp value) {
                colorOp = value;
                return *this;
            }

            RenderTargetBlendDesc &ColorSrcFactor(BlendFactor value) {
                colorSrcFactor = value;
                return *this;
            }

            RenderTargetBlendDesc &ColorDstFactor(BlendFactor value) {
                colorDstFactor = value;
                return *this;
            }

            RenderTargetBlendDesc &AlphaOp(BlendOp value) {
                alphaOp = value;
                return *this;
            }

            RenderTargetBlendDesc &AlphaSrcFactor(BlendFactor value) {
                alphaSrcFactor = value;
                return *this;
            }

            RenderTargetBlendDesc &AlphaDstFactor(BlendFactor value) {
                alphaDstFactor = value;
                return *this;
            }

            RenderTargetBlendDesc &Mask(ComponentSet value) {
                mask = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(
                        enabled,
                        colorOp, colorSrcFactor, colorDstFactor,
                        alphaOp, alphaSrcFactor, alphaDstFactor,
                        mask.Get()
                );
            }

            // Does this target need blending?
            bool enabled = true;

            // Color channels
            BlendOp colorOp = BlendOp::eAdd;
            BlendFactor colorSrcFactor = BlendFactor::eOne;
            BlendFactor colorDstFactor = BlendFactor::eOne;

            // Alpha channels
            BlendOp alphaOp = BlendOp::eAdd;
            BlendFactor alphaSrcFactor = BlendFactor::eOne;
            BlendFactor alphaDstFactor = BlendFactor::eOne;

            // Component mask
            ComponentSet mask = Component::eAll;
        };

        namespace BlendTargets {
            static auto noBlend = RenderTargetBlendDesc(
                    false,
                    BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eOne,
                    BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eOne,
                    Component::eAll
            );
            static RenderTargetBlendDesc add = RenderTargetBlendDesc(
                    true,
                    BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eOne,
                    BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eOne,
                    Component::eAll
            );
            static RenderTargetBlendDesc transperancy = RenderTargetBlendDesc(
                    true,
                    BlendOp::eAdd, BlendFactor::eSrcAlpha, BlendFactor::eOneMinusSrcAlpha,
                    BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eZero,
                    Component::eAll
            );
        }

        struct BlendStateDesc {
            BlendStateDesc() {
                coefficients.Resize(4);
                coefficients[0] = coefficients[1] = coefficients[2] = coefficients[3] = 1;
            }

            BlendStateDesc &Coefficients(const Core::FixedArray4<float> &value) {
                coefficients = value;
                return *this;
            }

            BlendStateDesc &RenderTargets(const Core::FixedArray8<RenderTargetBlendDesc> &value) {
                renderTargets = value;
                return *this;
            }

            BlendStateDesc &AddRenderTarget(const RenderTargetBlendDesc &value) {
                renderTargets.Add(value);
                return *this;
            }

            USize Hash() {
                USize hash = 0;
                for (auto coeff : coefficients) {
                    hash = CombineHash(hash, coeff);
                }
                for (auto target : renderTargets) {
                    hash = CombineHash(hash, target.Hash());
                }
                return hash;
            }

            // Blend coefficients
            Core::FixedArray4<float> coefficients;

            // Render target blend descriptions
            Core::FixedArray8<RenderTargetBlendDesc> renderTargets;
        };
    }
}