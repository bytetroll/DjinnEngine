#pragma once

#include <COM/Result.h>
#include <Host/Registry.h>
#include "ApplicationInfo.h"
#include <Platform/ConditionalVar.h>
#include <Platform/Mutex.h>
#include <Base/Pipeline.h>

namespace Djinn::Async {
    class Scheduler;
    class Framework;
}

namespace Djinn::Base {
    class ISystem;
    class IApplicationSystem;

    class DJINN_EXPORT_PLUGINS_BASE_APPLICATION Application : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        Application(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);
        Application(const Application &) = delete;
        Application(const Application &&) = delete;

        /// Initialize this application
        /// \return
        COM::Result Initialize(const ApplicationInfo &info);

        /// Register a system
        /// \param system
        /// \return
        COM::Result Register(IApplicationSystem *system);

        /// Deregister a system
        /// \param system
        /// \return
        COM::Result Deregister(IApplicationSystem *system);

        /// Run this application
        /// \param block - Wait until the application has finished
        /// \return
        COM::Result Run(bool block);

        /// Run this application
        /// \param block
        /// \param asyncLoad
        /// \return
        COM::Result Run(bool block, const Core::Delegate<void()> &asyncLoad);

        /// Get pipeline
        /// \return
        IPipeline *GetPipeline() const {
            return pipeline;
        }

        /// Get application info
        /// \return
        const ApplicationInfo &GetInfo() const {
            return info;
        }

    protected:
        /// Run invocation
        /// \param result
        void __Run(COM::Result *result, Core::Delegate<void()> asyncLoad);

    private:
        ApplicationInfo info;

        // Pipeline
        Pipeline *pipeline;

        // Block var
        Platform::ConditionalVar blockVar;
        Platform::Mutex blockMtx;

        // ...
        Async::Framework* asyncFramework = nullptr;
    };

    /// Launch application
    /// \param info
    /// \param callback
    /// \return
    extern DJINN_EXPORT_PLUGINS_BASE_APPLICATION COM::Result Launch(const Base::ApplicationInfo &info, const Core::Delegate<void(Base::Application &)> &callback);
}