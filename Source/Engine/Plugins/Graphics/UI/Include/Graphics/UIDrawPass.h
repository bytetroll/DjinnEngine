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
#include "IUIDrawPass.h"

namespace Djinn::Data {
    class ShaderAsset;
}

namespace Djinn::Graphics {
    class IUIProcess;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_UI UIDrawPass : public IUIDrawPass {
    public:
        DJINN_COM_CLASS();

        UIDrawPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIDrawPass(registry, outer, host) {

        }

        /// Initialize this pass
        /// \return
        COM::Result Initialize(IUIProcess *process);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        struct DrawVertex {
            Math::Vector2 position;
            Math::Vector2 texcoord;
            Int32 index;
        };

        Core::HashMap<Texture::Type::Type, UInt32> textureLUT;
        Data::Asset<Data::ShaderAsset> shaderAsset;
        VertexArray<DrawVertex> vertices;
        IndexArray<UInt32> indices;
        IUIProcess *process;
    };
}