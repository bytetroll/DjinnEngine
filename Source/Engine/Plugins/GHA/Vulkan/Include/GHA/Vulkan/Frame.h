//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include "FrameDesc.h"

namespace Djinn::GHA::Vulkan {
    class Pass;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Frame : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Frame(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const FrameDesc &desc, Pass *pass, vk::Device device);

        /// Overrides
        const FrameDesc &GetDesc();

        /// Get frame buffer
        /// \return
        const vk::Framebuffer &GetFrameBuffer() const {
            return frameBuffer;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Framebuffer frameBuffer;
        vk::Device device;
        FrameDesc desc;

    };
}