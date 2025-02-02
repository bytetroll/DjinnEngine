//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA {
    class IVertexBuffer;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN UnorderedVertexBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UnorderedVertexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, IVertexBuffer *vertexBuffer);

        /// Overrides
        IVertexBuffer *GetVertexBuffer();
        virtual COM::InterfaceID GetIID() override;

        /// Get descriptor
        /// \return
        const vk::DescriptorBufferInfo &GetDescriptor() const {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        IVertexBuffer *vertexBuffer;
        vk::DescriptorBufferInfo descriptor;
    };
}