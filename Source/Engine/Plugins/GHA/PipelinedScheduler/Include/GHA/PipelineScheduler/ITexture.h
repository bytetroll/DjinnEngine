//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/ITexture.h>

namespace Djinn::GHA::PipelineScheduler {
    class DJINN_EXPORT_PLUGINS_GHA_PIPELINESCHEDULER ITexture : public GHA::ITexture {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer 
        ITexture(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : GHA::ITexture(registry, outer, host) {

        }

        virtual ~ITexture() {

        }

        /// Current layouts
        TextureLayout layout[16] = {TextureLayout::eUndefined};
    };
}