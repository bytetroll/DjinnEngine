//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA::Vulkan {
    class StructuredBufferView;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN UnorderedStructuredBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UnorderedStructuredBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, StructuredBufferView *structuredBufferView);

        /// Overrides
        StructuredBufferView *GetStructuredBufferView();
        virtual COM::InterfaceID GetIID() override {
            return kIID;
        }

        /// Get descriptor
        /// \return
        const vk::DescriptorBufferInfo &GetDescriptor() const {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        StructuredBufferView *structuredBufferView;
        vk::DescriptorBufferInfo descriptor;
    };
}