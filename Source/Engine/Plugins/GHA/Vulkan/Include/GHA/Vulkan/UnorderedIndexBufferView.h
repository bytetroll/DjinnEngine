//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/IResource.h>

namespace Djinn::GHA::Vulkan {
    class IndexBufferView;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN UnorderedIndexBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UnorderedIndexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device, IndexBufferView *indexBufferView);

        /// Overrides
        IndexBufferView *GetIndexBufferView();
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
        IndexBufferView *indexBufferView;
        vk::BufferView view;
    };
}