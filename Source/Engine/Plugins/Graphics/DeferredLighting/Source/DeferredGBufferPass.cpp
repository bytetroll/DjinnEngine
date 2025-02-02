//
// Created by Dev on 2/26/2018.
//

#include <Graphics/ILightingProcess.h>
#include <Graphics/PacketBatch.h>
#include <Math/Matrix4.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/ShaderConfiguration.h>
#include <Data/ShaderAsset.h>
#include <Data/IMeshAsset.h>
#include <Data/IAssetHost.h>
#include <Graphics/IOutput.h>
#include <Graphics/DeferredGBufferPass.h>
#include <Graphics/CameraPacket.h>
#include <Graphics/SceneMeshPacket.h>
#include <GHA/ITexture.h>
#include <Data/IMaterialAsset.h>
#include <Graphics/DeferredGBufferMaterialType.h>

DJINN_NS2(Graphics);

COM::Result DeferredGBufferPass::Initialize(ILightingProcess *process) {
    this->process = process;

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("UI draw pass expected an asset worldHost to be installed");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result DeferredGBufferPass::Preprocess(PassBuilder builder) {
    // Get target
    auto target = process->GetPacket()->output->GetTexture(builder);

    // Create gbuffer
    {
        gbufferDepth = builder.AddTexture(
                GHA::TextureDesc()
                        .Format(GHA::Format::eD32F)
                        .Usage(GHA::TextureUsage::eDepthStencil)
                        .Size(target->size)
                        .Flags(GHA::TextureFlag::eClearable)
        );
        gbuffer0 = builder.AddTexture(
                GHA::TextureDesc()
                        .Format(GHA::Format::eR8G8B8A8Unorm)
                        .Usage(GHA::TextureUsage::eColor)
                        .Size(target->size)
                        .Flags(GHA::TextureFlag::eClearable)
        );
        gbuffer1 = builder.AddTexture(
                GHA::TextureDesc()
                        .Format(GHA::Format::eA2R10G10B10Unorm)
                        .Usage(GHA::TextureUsage::eColor)
                        .Size(target->size)
                        .Flags(GHA::TextureFlag::eClearable)
        );
        gbuffer2 = builder.AddTexture(
                GHA::TextureDesc()
                        .Format(GHA::Format::eR8G8B8A8Unorm)
                        .Usage(GHA::TextureUsage::eColor)
                        .Size(target->size)
                        .Flags(GHA::TextureFlag::eClearable)
        );

        // Map
        builder.MapSharedTexture("GBuffer.Depth"_id, gbufferDepth, GHA::TextureViews::none, GHA::Samplers::noMip);
        builder.MapSharedTexture("GBuffer.0"_id, gbuffer0, GHA::TextureViews::none, GHA::Samplers::noMip);
        builder.MapSharedTexture("GBuffer.1"_id, gbuffer1, GHA::TextureViews::none, GHA::Samplers::noMip);
        builder.MapSharedTexture("GBuffer.2"_id, gbuffer2, GHA::TextureViews::none, GHA::Samplers::noMip);

        // Clear
        builder.ClearDepthStencil(gbufferDepth);
        builder.ClearColor(gbuffer0);
        builder.ClearColor(gbuffer1);
        builder.ClearColor(gbuffer2);
    }

    // OK
    return COM::kOK;
}

COM::Result DeferredGBufferPass::Build(PassBuilder builder, double delta) {
    // Get resources
    // auto& resources = process->GetResources();

    // Get camera packet
    auto camera = process->GetPacket();
    if (!camera->output) {
        // TODO: Logging of sorts?
        return COM::kOK;
    }

    // Get target
    // auto target = process->GetPacket()->output->GetTexture(builder);

    // Get buffer
    auto buffer = camera->batch.GetPacket<SceneMeshPacketBuffer>();

    // Camera data
    struct CameraData {
        Math::Matrix4 view;
        Math::Matrix4 projection;
        Math::Matrix4 viewProjection;
    } cameraData = {
            camera->view.Transposed(),
            camera->projection.Transposed(),
            (camera->view * camera->projection).Transposed()
    };

    // Instance data
    struct InstanceData {
        Math::Matrix4 transform;
    };

    // Batch data
    struct BatchData {
        InstanceData instances[512];
    } batchData;

    // Prepare common settings
    builder.BindPacked("Data.scene"_id, &cameraData);
    builder.BindRenderTarget(0, gbuffer0, GHA::TextureViews::none, GHA::BlendTargets::noBlend);
    builder.BindRenderTarget(1, gbuffer1, GHA::TextureViews::none, GHA::BlendTargets::noBlend);
    builder.BindRenderTarget(2, gbuffer2, GHA::TextureViews::none, GHA::BlendTargets::noBlend);
    builder.BindDepthRenderTarget(gbufferDepth, GHA::TextureViews::none);
    builder.SetViewport(GHA::Rect(gbufferDepth->size));
    builder.SetScissor(GHA::Rect(gbufferDepth->size));

    // Collect all packets
    auto &packets = buffer->GetPackets();

    // Sort
    packets.Sort([](SceneMeshPacket &a, SceneMeshPacket &b) { return (a.material.Get() < b.material.Get()) || (a.material.Get() == b.material.Get() && a.mesh.Get() < b.mesh.Get()); });

    // Indices
    Int32 instance = 0;

    // Current states
    Data::IMaterialAsset* material = nullptr;
    Data::IMeshAsset* mesh = nullptr;

    // Current mesh data
    Mesh meshData;

    // Update constants
    builder.BindDynamic("Data.batchIndex"_id, &instance);

    // Process all packets
    for (USize packetIndex = 0; packetIndex < packets.Size(); packetIndex++) {
        auto &packet = packets[packetIndex];

        // First instance?
        if (instance == 0) {
            for (UInt32 offset = 0; offset < Math::Min(512u, packets.Size() - packetIndex); offset++) {
                batchData.instances[offset].transform = packets[packetIndex + offset].transform.Transposed();
            }
            builder.BindPacked("Data.batch"_id, &batchData);
        }

        // New material?
        if (material != packet.material.Get()) {
            material = packet.material.Get();

            // Query material
            Data::ShaderAsset *shader;
            if (!material->GetShader(
                    Data::MaterialQuery()
                            .Layout(packet.mesh->GetLayoutClassID())
                            .Type(DeferredGBufferMaterialType::kCID),
                    &shader
            )) {
                // Failed to query shader, invalid combination
                // TODO: Warn somehow? Maybe an incremental counter
                continue;
            }

            // Bind
            builder.BindShader(shader);
        }

        // Add mesh
        if (mesh != packet.mesh.Get()) {
            mesh = packet.mesh.Get();

            // Construct mesh
            meshData = builder.AddMesh(packet.mesh);

            // Bind
            builder.BindVertexBuffer(0, meshData.vertexBuffer);
            builder.BindIndexBuffer(meshData.indexBuffer);
        }

        // Update constants
        builder.BindDynamic("Data.batchIndex"_id, &instance);

        // Draw
        builder.DrawIndexed(static_cast<UInt32>(meshData.indexBuffer->count));

        if (instance >= 512) {
            instance = 0;
        } else {
            instance++;
        }
    }

    // Testing.
    // builder.CopyTexture(gbuffer1, builder.AddTexture(camera->output->GetTexture()));

    // OK
    return COM::kOK;
}

COM::Result DeferredGBufferPass::Configure() {
    // OK
    return COM::kOK;
}

