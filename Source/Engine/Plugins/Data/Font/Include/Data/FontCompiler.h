//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Core/IStream.h>
#include <Core/Atomic.h>
#include "IFontCompiler.h"
#include "FontConfiguration.h"

namespace Djinn::Data {
    class BaseFontAsset;

    class DJINN_EXPORT_PLUGINS_DATA_FONT FontCompiler : public IFontCompiler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        FontCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        ~FontCompiler();

        /// Initialize the compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
            };
}