//
// Created by Dev on 2/24/2018.
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