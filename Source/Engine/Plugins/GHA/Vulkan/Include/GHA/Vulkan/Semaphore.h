//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Semaphore : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Semaphore(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device);

        /// Get Semaphore
        /// \return
        const vk::Semaphore &GetSemaphore() const {
            return semaphore;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Device device;
        vk::Semaphore semaphore;
    };
}