//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA {
    class IIndexBuffer;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN IndexBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        IndexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, IIndexBuffer *indexBuffer);

        /// Overrides
        IIndexBuffer *GetIndexBuffer();
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
        IIndexBuffer *indexBuffer;
        vk::Device device;
        vk::BufferView view;
    };
}