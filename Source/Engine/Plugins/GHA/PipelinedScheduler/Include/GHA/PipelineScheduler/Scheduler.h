//
// Created by Dev on 12/24/2017.
//
#pragma once

#include <GHA/IScheduler.h>
#include <Core/Array.h>
#include <Core/HashMap.h>
#include <GHA/Commands.h>
#include <Core/HashSet.h>
#include <Core/SlotArray.h>
#include <GHA/SchedulerContext.h>
#include <Core/Pool.h>
#include <Core/SparseArray.h>
#include <Core/Map.h>
#include <Core/IndexArray.h>
#include "Command.h"

namespace Djinn::GHA {
    class IResourceStager;
    class IConstantBuffer;
    class IDevice;
    class ISampler;
    class IMemoryResource;
}

namespace Djinn::GHA::PipelineScheduler {
    class ITexture;

    class Scheduler : public IScheduler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Scheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::Result Initialize(IDevice *device, ISwapchain *swapchain) override;
        virtual COM::Result Schedule(UInt32 count, ScheduledNode **nodes) override;
        virtual COM::Result Compile() override;
        virtual SchedulerContext *GetContext() override;

    protected:
        /// Overrides
        COM::Result Released() override;

    public:

        /// Get active queue count
        /// \return
        USize GetQueueCount() const {
            return queues.Size();
        }

        /// Enumerate all commands
        /// \param index
        /// \param queue
        /// \param commands
        /// \return
        COM::Result EnumerateQueue(USize index, UInt32 &commandCount, PipelineScheduler::Command **commands) {
            if (index >= queues.Size()) {
                return COM::kError;
            }
            commandCount = static_cast<UInt32>(queues[index].commands.Size());
            *commands = queues[index].commands.Ptr();
            return COM::kOK;
        }

        COM::Result EnumerateResources(UInt32 &count, PipelineScheduler::ResourceCommand **commands) {
            count = static_cast<UInt32>(resources.Size());
            *commands = resources.Ptr();
            return COM::kOK;
        }

    private:
        /// Meta data of previously submitted frame
        struct FrameData {
            /// Used memory resources for later freeing
            Core::Buffer<GHA::IMemoryResource *> memoryResources;

            /// Deferred memory resources for later releasing
            Core::Buffer<GHA::IMemoryResource *> deferredDeletes;

            /// Descriptor sets for later retiring
            struct DescriptorSet {
                USize bucket;
                PersistentID id;
            };
            Core::Buffer<DescriptorSet> descriptorSets;
        };

        /// Compiled queue data
        struct QueueBucket {
            Core::Buffer<PipelineScheduler::Command> commands;
        };

        /// Texture meta
        struct IntermediateTextureDesc {
            bool isBound = false;
            PipelineScheduler::ITexture *texture;
            TextureDesc desc;
        };

        /// Texture view meta
        struct IntermediateTextureViewDesc {
            PipelineScheduler::ITexture *texture;
            GHA::TextureID textureID;
            TextureViewDesc desc;
        };

        struct StreamableTextureDesc {
            PipelineScheduler::ITexture *texture;
            Core::FixedArray16<USize> commitIndices;
        };

        struct StreamedVertexDesc {
            USize NextIndex() {
                USize idx = bufferIndex++;
                if (bufferIndex >= buffers.Size()) {
                    bufferIndex = 0;
                }
                return idx;
            }

            Core::Array<GHA::IVertexBuffer *> buffers;
            USize bufferIndex = 0;
            USize size = 0;
            USize commitIndex = 0;
            bool initialized = false;
        };

        struct StreamedIndexDesc {
            USize NextIndex() {
                USize idx = bufferIndex++;
                if (bufferIndex >= buffers.Size()) {
                    bufferIndex = 0;
                }
                return idx;
            }

            Core::Array<GHA::IIndexBuffer *> buffers;
            USize bufferIndex = 0;
            USize size = 0;
            USize commitIndex = 0;
            bool initialized = false;
        };

        template<typename DESC, typename ID>
        struct State {
            void Flush() {
                dirty = true;
                desc = {};
            }

            void Dirty() {
                dirty = true;
            }

            void Clean() {
                dirty = false;
            }

            bool dirty = true;
            DESC desc;
            ID id;
        };

    private:
        COM::Result GarbageCollect(FrameData *frame);

        COM::Result Flush(FrameData *frame);

        COM::Result AnalyzeMutableResourceLifetime(FrameData *frame);

        COM::Result AllocateResources(QueueBucket *bucket, FrameData *frame);

        COM::Result GenerateCommands(FrameData *frame);

        COM::Result CompileNode(FrameData *frame, QueueBucket *bucket, ScheduledNode *node);

        void FlushState(FrameData *frame, QueueBucket *bucket);

        void FlushLayout(QueueBucket *bucket);

        void EndRenderWork(QueueBucket *bucket);

    private:

        void Barrier(QueueBucket *queue, const TextureViewID &uid, TextureLayout to);

        TextureID ImportTexture(GHA::ITexture *texture);

        TextureID ImportTexture(GHA::ITexture *texture, TextureID id);

        TextureViewID CreateTextureView(TextureID id, const TextureViewDesc &desc);

        PersistentID CreatePersistent(ResourceID id);

        ResourceID LoadPersistent(PersistentID id);

        /// Scheduled Node Buckets
        Core::SlotArray<ScheduledNode *> scheduledNodes;
        Core::Buffer<ScheduledNode *> sortedNodes;
        Core::HashSet<ScheduledNode *> completedNodes;
        Core::HashSet<ScheduledNode *> failedNodes;

        Core::SparseArray<IntermediateTextureDesc> textureDescs;
        Core::SparseArray<IntermediateTextureViewDesc> textureViewDescs;
        Core::HashMap<ITexture *, ITexture *> textureMs;

        /// Frames
        Core::Array<FrameData> frames;

        /// Commands
        Core::Array<QueueBucket> queues;
        Core::Buffer<PipelineScheduler::ResourceCommand> resources;

        /// Dummy objects that acts as a placeholder
        ITexture *dummyTexture = nullptr;
        IVertexBuffer *dummyVertexBuffer = nullptr;
        IIndexBuffer *dummyIndexBuffer = nullptr;

        /// Hashed streamed objects
        Core::HashMap<USize, StreamableTextureDesc> streamedTextures;
        Core::HashMap<USize, StreamedVertexDesc> streamedVertices;
        Core::HashMap<USize, StreamedIndexDesc> streamedIndices;

        Core::FixedArray8<VertexBufferID> boundVertexBuffers;
        IndexBufferID boundIndexBuffer;

        /// On demand textures
        struct ODTexture {
            struct Object {
                ITexture *texture = nullptr;
                Int32 gcCounter = 0;
            };
            UInt32 index;
            Core::Array<Object> objects;
        };
        Core::ObjectPool<ODTexture> odTexturePool;
        Core::HashMap<USize, ODTexture *> odTextures;

        /// Shaders
        Core::HashMap<USize, IConstantBuffer *> constantBuffers;
        Core::HashMap<ShaderBlob *, IShader *> shaders;

        /// Current bind states
        struct BindState {
            void Flush() {
                dirty = true;
                binds.Clear();
                arrayBinds.Clear();
                ids.Clear();
            }

            void Dirty() {
                dirty = true;
                barrierDirty = true;
            }

            void BarrierDirty() {
                barrierDirty = true;
            }

            void Clean() {
                dirty = false;
            }

            void BarrierClean() {
                barrierDirty = false;
            }

            bool dirty = true;
            bool barrierDirty = false;
            Core::HashMap<BindID, DescriptorBinding> binds;
            Core::HashMap<BindID, Core::FixedArray64<DescriptorBinding>> arrayBinds;
            Core::FixedArray32<ResourceID> ids;
        } bindState;

        /// Current states
        State<PipelineDesc, ResourceID> pipelineState;
        State<FrameDesc, FrameID> frameState;
        State<PassDesc, PassID> passState;
        PassID lastPass = nullptr;

        /// Binding basemap
        struct PipelineLayoutBindingTemplate {
            Core::Map<BindID, UInt32> offsets;
            Core::FixedArray12<DescriptorSetUpdateDesc> descs;
        };
        Core::LazyObjectPool<PipelineLayoutBindingTemplate> templatePool;

        /// Current bindings
        ResourceID boundPipeline;
        Core::FixedArray16<ResourceID> boundDescriptorSets;

        struct DescriptorSetBucket {
            Core::SlotArray<PersistentID> retiredSets;
        };
        Core::ObjectPool<DescriptorSetBucket> descriptorBucketPool;
        Core::Map<USize, DescriptorSetBucket *> descriptorBuckets;

        struct CompiledShaderLayout {
            Core::FixedArray8<DescriptorSetLayoutDesc> layoutDescs;
            PipelineLayoutBindingTemplate *descriptorLayoutTemplate = nullptr;
            State<PipelineLayoutDesc, ResourceID> pipelineLayoutState;

            /// Current pipeline bind state
            struct PipelineBindState {
                Core::HashMap<BindID, PipelineBinding> map;
                Core::HashMap<UInt64, BindID> mapR;
            } pipelineBindState;

            /// Current dynamic bind state
            struct PipelineDynamicState {
                Core::HashMap<BindID, StageSet> stageVisibility;
                Core::HashMap<BindID, UInt32> offsets;
            } pipelineDynamicState;
        };


        /// Current dynamic bind state
        struct DynamicBindState {
            void Flush() {
                dirty = true;
                map.Clear();
            }

            void Dirty() {
                dirty = true;
            }

            void Clean() {
                dirty = false;
            }

            bool dirty = true;
            Core::HashMap<BindID, Core::Pair<void *, USize>> map;
        } dynamicState;

        Core::HashMap<USize, CompiledShaderLayout> compiledShaderLayouts;
        CompiledShaderLayout *currentShaderLayout = nullptr;

        /// Persistent resources
        struct PersistentResource {

        };
        Core::IndexArray<PersistentResource> persistentResources;

        /// Handles
        IDevice *device = nullptr;
        ISwapchain *swapchain = nullptr;
        IResourceStager *stager = nullptr;

        /// General context
        SchedulerContext context;

        /// Pools
        Core::LazyObjectPool<Core::FixedArray16<Command::CmdTextureBarrier::Barrier>> barrierPools;
        Core::LazyObjectPool<DescriptorSetLayoutDesc> descriptorSetLayoutPools;
        Core::LazyObjectPool<DescriptorSetUpdateDesc> descriptorSetUpdatePools;
        Core::LazyObjectPool<PassDesc> passPools;
        Core::LazyObjectPool<FrameDesc> framePools;
        Core::LazyObjectPool<PipelineDesc> pipelinePools;

        Core::HashMap<const char *, UInt32> counts;

        /// Garbage collection threshold
        UInt32 gcCounterTreshold = 10u;
    };
}