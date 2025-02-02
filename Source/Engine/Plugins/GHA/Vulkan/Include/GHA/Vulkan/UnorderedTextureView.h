//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA::Vulkan {
    class TextureView;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN UnorderedTextureView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UnorderedTextureView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, TextureView *textureView);

        /// Overrides
        TextureView *GetTextureView();
        virtual COM::InterfaceID GetIID() override;

        /// Get view
        /// \return
        const vk::ImageView &GetView() const {
            return view;
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
        TextureView *textureView;
        vk::ImageView view;
        vk::DescriptorImageInfo descriptor;
    };
}