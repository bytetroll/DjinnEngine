#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>
#include "SampleMode.h"
#include "RenderTargetOp.h"
#include "TextureLayout.h"

namespace Djinn {
    namespace GHA {
        enum class Format : Int32;

        struct RenderTargetDesc {
            RenderTargetDesc& Index(UInt32 value) {
                index = value;
                return *this;
            }

            RenderTargetDesc& Format(GHA::Format value) {
                format = value;
                return *this;
            }

            RenderTargetDesc& ColorLoadOp(RenderTargetLoadOp value) {
                colorLoadOp = value;
                return *this;
            }

            RenderTargetDesc& ColorStoreOp(RenderTargetWriteOp value) {
                colorStoreOp = value;
                return *this;
            }

            RenderTargetDesc& StencilLoadOp(RenderTargetLoadOp value) {
                stencilLoadOp = value;
                return *this;
            }

            RenderTargetDesc& StencilStoreOp(RenderTargetWriteOp value) {
                stencilStoreOp = value;
                return *this;
            }

            RenderTargetDesc& InitialLayout(TextureLayout value) {
                initialLayout = value;
                return *this;
            }

            RenderTargetDesc& FinalLayout(TextureLayout value) {
                finalLayout = value;
                return *this;
            }

            RenderTargetDesc& SampleMode(GHA::SampleMode value) {
                sampleMode = value;
                return *this;
            }

            USize Hash() {
                return CombineHash(
                        index, format,
                        colorLoadOp, colorStoreOp,
                        stencilLoadOp, stencilStoreOp,
                        initialLayout, finalLayout,
                        sampleMode
                );
            }

            // Index this render target binds to
            UInt32 index;

            // Format of this render target
            GHA::Format format;

            // Color operations
            RenderTargetLoadOp colorLoadOp = RenderTargetLoadOp::eLoad;
            RenderTargetWriteOp colorStoreOp = RenderTargetWriteOp::eWrite;

            // Stencil operations
            RenderTargetLoadOp stencilLoadOp = RenderTargetLoadOp::eLoad;
            RenderTargetWriteOp stencilStoreOp = RenderTargetWriteOp::eWrite;

            // Layouts
            TextureLayout initialLayout = TextureLayout::eColorRenderTarget;
            TextureLayout finalLayout = TextureLayout::eColorRenderTarget;

            // Sample mode
            GHA::SampleMode sampleMode = GHA::SampleMode::e1;
        };
    }
}