//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA {
    class IStructuredBuffer;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN StructuredBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        StructuredBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, IStructuredBuffer *StructuredBuffer);

        /// Overrides
        IStructuredBuffer *GetStructuredBuffer();
        virtual COM::InterfaceID GetIID() override {
            return kIID;
        }

        /// Get view
        /// \return
        const vk::DescriptorBufferInfo &GetDescriptor() const {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        IStructuredBuffer *structuredBuffer;
        vk::DescriptorBufferInfo descriptor;
    };
}