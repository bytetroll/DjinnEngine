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
 on 2/24/2018.
//
#pragma once

#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Graphics/ILightingPass.h>
#include <Data/Asset.h>

namespace Djinn::Data {
    class ShaderAsset;
}

namespace Djinn::Graphics {
    class ILightingProcess;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING DeferredDirectionalLightPass : public ILightingPass {
    public:
        DJINN_COM_CLASS();

        DeferredDirectionalLightPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILightingPass(registry, outer, host) {

        }

        /// Initialize this pass
        /// \return
        COM::Result Initialize(ILightingProcess* process);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        Data::Asset<Data::ShaderAsset> shaderAsset;
        ILightingProcess* process;
    };
}