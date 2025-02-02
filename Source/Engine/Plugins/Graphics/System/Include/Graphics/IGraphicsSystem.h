//
// Created by Dev on 1/10/2018.
//
#pragma once

#include <Game/IStepSystem.h>

namespace Djinn::GHA {
    class IInstance;
    class IDevice;
    class ISwapchain;
    class IConfiguration;
    class IScheduler;
    class IInterpreter;
    class IResourceStager;
}

namespace Djinn::Platform {
    class Window;
}

namespace Djinn::Graphics {
    class IGraph;
    class PacketBatch;

    /// Common Results
    DJINN_COM_RESULT(NoAdapters, true, "Graphics");

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SYSTEM IGraphicsSystem : public Game::IStepSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IGraphicsSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IStepSystem(registry, outer, host) {

        }

        /// Configure this graphics system
        /// \param configuration
        /// \return
        using Djinn::Base::ISystem::Configure;
        virtual COM::Result Configure(GHA::IConfiguration* configuration, Platform::Window* window) = 0;

        /// Get graph
        /// \return
        virtual IGraph* GetGraph() = 0;

        /// Get configuration
        /// \return
        virtual GHA::IConfiguration* GetConfiguration() = 0;

        /// Get scheduler
        /// \return
        virtual GHA::IScheduler* GetScheduler() = 0;

        /// Get interpreter
        /// \return
        virtual GHA::IInterpreter* GetInterpreter() = 0;

        /// Get instance
        /// \return
        virtual GHA::IInstance* GetInstance()  = 0;

        /// Get device
        /// \return
        virtual GHA::IDevice* GetDevice()  = 0;

        /// Get swapchain
        /// \return
        virtual GHA::ISwapchain* GetSwapchain()  = 0;
    };
}