//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "ISceneProcess.h"

namespace Djinn::Graphics {
    class Pipeline;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SCENE SceneProcess : public ISceneProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        SceneProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Build(ScenePacket* batch, Graphics::PassBuilder builder, double delta) override;
        virtual IPipeline *GetPipeline() override;
        virtual ScenePacket *GetPacket() override;

    private:
        ScenePacket* packet;
        Pipeline* pipeline = nullptr;
        IGraphicsSystem* system;
    };
}