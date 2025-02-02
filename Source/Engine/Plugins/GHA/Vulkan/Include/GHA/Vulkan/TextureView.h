//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/TextureViewDesc.h>
#include <GHA/IResource.h>

namespace Djinn::GHA {
    class ITexture;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN TextureView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const TextureViewDesc &desc, vk::Device device, ITexture *texture);

        /// Overrides
        const TextureViewDesc &GetDesc();
        ITexture *GetTexture();
        virtual COM::InterfaceID GetIID() override {
            return kIID;
        }

        /// Get view
        /// \return
        const vk::ImageView &GetView() const {
            return view;
        }

        /// Get texture
        /// \return
        ITexture *GetTexture() const {
            return texture;
        }

        /// Get descriptor
        /// \return
        const vk::DescriptorImageInfo &GetDescriptor() {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        TextureViewDesc desc;
        ITexture *texture;
        vk::ImageView view = {};
        vk::Device device;
        vk::DescriptorImageInfo descriptor;
    };
}