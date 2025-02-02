//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "IUIMLCompiler.h"

namespace Djinn::Data {
    class UIMLConfiguration;

    class DJINN_EXPORT_PLUGINS_DATA_UIML UIMLCompiler : public IUIMLCompiler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UIMLCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize the compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
    };
}