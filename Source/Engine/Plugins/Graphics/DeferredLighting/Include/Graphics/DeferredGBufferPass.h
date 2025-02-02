//
// Created by Dev on 2/24/2018.
//
#pragma once

#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include "IDeferredGBufferPass.h"

namespace Djinn::Data {
    class ShaderAsset;
}

namespace Djinn::Graphics {
    class ILightingProcess;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING DeferredGBufferPass : public IDeferredGBufferPass {
    public:
        DJINN_COM_CLASS();

        DeferredGBufferPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IDeferredGBufferPass(registry, outer, host) {

        }

        /// Initialize this pass
        /// \return
        COM::Result Initialize(ILightingProcess* process);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        ILightingProcess* process;
        Texture gbufferDepth;
        Texture gbuffer0;
        Texture gbuffer1;
        Texture gbuffer2;
    };
}