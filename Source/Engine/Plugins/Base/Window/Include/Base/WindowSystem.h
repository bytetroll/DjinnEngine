//
// Created by Dev on 2/25/2018.
//
#pragma once

#include "IWindowSystem.h"
#include <Platform/Window.h>

namespace Djinn::Base {
    class WindowSystem : public IWindowSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        WindowSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \param desc
        /// \return
        COM::Result Initialize(const Platform::WindowDesc &desc, bool focus = true);

        /// Overrides
        virtual Platform::Window *GetWindow() override;
        virtual COM::Result Run() override;
        virtual COM::Result Configure(PipelineBuilder& builder) override;

    private:
        Platform::Window window;
    };
}