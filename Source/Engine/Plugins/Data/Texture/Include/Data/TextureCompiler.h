//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Core/IStream.h>
#include <Core/Atomic.h>
#include "ITextureCompiler.h"
#include "TextureConfiguration.h"

namespace Djinn::Data {
    class BaseTextureAsset;

    class DJINN_EXPORT_PLUGINS_DATA_TEXTURE TextureCompiler : public ITextureCompiler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        ~TextureCompiler();

        /// Initialize the compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
            };
}