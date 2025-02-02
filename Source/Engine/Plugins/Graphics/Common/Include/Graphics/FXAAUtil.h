//
// Created by Dev on 3/6/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Graphics/ID.h>
#include <Graphics/PassBuilder.h>

namespace Djinn::Data {
    class ShaderAsset;
}

namespace Djinn::Graphics {
    class DJINN_EXPORT_PLUGINS_GRAPHICS_COMMON FXAAUtil : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        FXAAUtil(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this util
        /// \return
        COM::Result Initialize();

        /// Perform a FXAA resolve on source into dest
        /// \param builder
        /// \param source
        /// \param dest
        /// \return
        COM::Result Build(PassBuilder builder, const Texture& source, const Texture& dest);

        /// Overrides

    private:
        Data::Asset<Data::ShaderAsset> shaderAsset;
    };
}