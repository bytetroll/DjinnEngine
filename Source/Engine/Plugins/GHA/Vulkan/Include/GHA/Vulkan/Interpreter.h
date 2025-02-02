//
// Created by Dev on 12/21/2017.
//
#pragma once

#include <GHA/IInterpreter.h>
#include <Core/Array.h>
#include <GHA/ID.h>
#include <Core/HashMap.h>
#include <vulkan/vulkan.hpp>
#include <Core/Atomic.h>
#include <Core/SparseArray.h>
#include <Core/Map.h>
#include <Core/SlotArray.h>

namespace Djinn::GHA {
    class IResource;
    class ITexture;
}

namespace Djinn::GHA::Vulkan {
    class CommandList;
    class CommandPool;
    class Device;
    class Swapchain;
    class Queue;
    class Pipeline;
    class DescriptorSet;
    class DescriptorSetLayout;
    class Pass;
    class Frame;
    class TextureView;
    class PipelineLayout;
    class Sampler;
    class Semaphore;
    class Instance;
    class Adapter;

    class Interpreter : public IInterpreter {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Interpreter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Initialize(IInstance *instance, IAdapter* adapter, IDevice *device, ISwapchain *swapchain) override;
        virtual COM::Result Compile(IScheduler *scheduler) override;
        virtual double GetGPUDelta() override;

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        Core::Pair<UInt32, UInt32> GetStampRange();

        Instance *instance;
        Adapter *adapter;
        Device *device;
        Swapchain *swapchain;

        struct QueueData {
            UInt32 queueIndex = 0;
            struct SubmissionData {
                CommandList *list;
                CommandPool *pool;
            };
            Core::Array<SubmissionData> submissions;
        };
        Core::Array<QueueData> queueData;
        Core::Array<Queue *> queues;

        Core::SparseArray<COM::IUnknown *> resources;

        Core::Map<USize, ITexture *> textures;
        Core::Map<USize, TextureView *> textureViews;
        Core::Map<USize, Pipeline *> pipelines;
        Core::Map<USize, PipelineLayout *> pipelineLayouts;
        Core::Map<USize, Frame *> frames;
        Core::Map<USize, Pass *> passes;
        Core::Map<USize, Sampler *> samplers;
        Core::Map<USize, DescriptorSetLayout *> descriptorSetLayouts;
        Core::Map<USize, DescriptorSet *> descriptorSets;

        Core::SparseArray<COM::IUnknown *> persistents;

        vk::QueryPool queryPool;
        UInt32 stampOffset = 0;
        Core::Array<UInt32> queryStamps;

        Core::Atomic<double> gpuDelta{0};

        Semaphore *renderComplete;
    };
}