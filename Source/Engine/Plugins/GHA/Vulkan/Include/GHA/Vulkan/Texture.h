//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <Core/Atomic.h>
#include <GHA/PipelineScheduler/ITexture.h>

namespace Djinn::GHA {
    class ICapabilities;
}

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Texture : public PipelineScheduler::ITexture {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Texture(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize
        /// \return
        COM::Result Initialize(const TextureDesc &desc, vk::Device device, Allocator *allocator, ICapabilities *capabilities);

        /// Initialize from existing image
        /// \return
        COM::Result Initialize(const TextureDesc &desc, vk::Image image, vk::Device device);

        /// Overrides
        virtual COM::Result Map(void **out, const Core::Optional<MemoryBounds> &bounds) override;
        virtual COM::Result Unmap(void *ptr) override;
        const TextureDesc &GetDesc() final;
        USize GetSize() final;

    protected:
        /// Overrides
        COM::Result Released() override;

    public:

        /// Get image
        /// \return
        const vk::Image &GetImage() const {
            return image;
        }

        /// Get allocation
        /// \return
        Allocation *GetAllocation() const {
            return alloc;
        }

        /// Get pixel size
        /// \return
        USize GetPixelSize() const {
            return formatSize;
        }

        vk::MemoryRequirements GetRequirements() const {
            return requirements;
        }

    private:
        Allocation *alloc;
        vk::MemoryRequirements requirements;
        TextureDesc desc;
        vk::Device device;
        Allocator *allocator = nullptr;
        vk::Image image;
        USize formatSize = 0;
    };
}