//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA::Vulkan {
    class TextureBufferView;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN UnorderedTextureBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UnorderedTextureBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, TextureBufferView *textureBufferView);

        /// Overrides
        TextureBufferView *GetTextureBufferView();
        virtual COM::InterfaceID GetIID() override;

        /// Get view
        /// \return
        const vk::BufferView &GetView() const {
            return view;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        TextureBufferView *textureBufferView;
        vk::BufferView view;
    };
}