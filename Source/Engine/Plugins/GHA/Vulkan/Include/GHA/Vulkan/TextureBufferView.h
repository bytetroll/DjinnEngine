//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/TextureBufferViewDesc.h>

namespace Djinn::GHA {
    class ITextureBuffer;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN TextureBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const TextureBufferViewDesc &desc, vk::Device device, ITextureBuffer *TextureBuffer);

        /// Overrides
        ITextureBuffer *GetTextureBuffer();
        const TextureBufferViewDesc &GetDesc();
        virtual COM::InterfaceID GetIID() override {
            return kIID;
        }

        /// Get view
        /// \return
        const vk::BufferView &GetView() const {
            return view;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        ITextureBuffer *textureBuffer;
        TextureBufferViewDesc desc;
        vk::Device device;
        vk::BufferView view;
    };
}