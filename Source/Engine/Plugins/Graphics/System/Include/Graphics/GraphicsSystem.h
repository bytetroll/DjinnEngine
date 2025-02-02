//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Core/Timer.h>
#include <Core/Array.h>
#include "IGraphicsSystem.h"

namespace Djinn::Graphics {
    class Graph;
    class PacketBatch;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SYSTEM GraphicsSystem : public IGraphicsSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        GraphicsSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Configure(GHA::IConfiguration *configuration, Platform::Window *window) override;
        virtual IGraph *GetGraph() override;
        virtual GHA::IConfiguration *GetConfiguration() override;
        virtual GHA::IScheduler *GetScheduler() override;
        virtual GHA::IInterpreter *GetInterpreter() override;
        virtual COM::Result Run(Game::StepMode mode, double delta) override;
        virtual Game::StepModeSet GetStepMode() override;
        virtual GHA::IInstance *GetInstance() override;
        virtual GHA::IDevice *GetDevice() override;
        virtual GHA::ISwapchain *GetSwapchain() override;
        virtual COM::Result Configure(Base::PipelineBuilder& builder) override;

    private:
        GHA::IConfiguration *configuration = nullptr;
        GHA::IScheduler *scheduler = nullptr;
        GHA::IInterpreter *interpreter = nullptr;
        GHA::IInstance *instance = nullptr;
        GHA::IDevice *device = nullptr;
        GHA::ISwapchain *swapchain = nullptr;
        Platform::Window *window = nullptr;
        Graph *graph = nullptr;
    };
}