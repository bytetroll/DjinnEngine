//
// Created by Dev on 2/26/2018.
//

#include <Graphics/DeferredDirectionalLightPass.h>
#include <Graphics/ILightingProcess.h>
#include <Graphics/IOutput.h>
#include <Graphics/PacketBatch.h>
#include <Math/Matrix4.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/ShaderConfiguration.h>
#include <Data/ShaderAsset.h>
#include <Data/IAssetHost.h>
#include <Graphics/SceneDirectionalLightPacket.h>
#include <Graphics/CameraPacket.h>

DJINN_NS2(Graphics);

COM::Result DeferredDirectionalLightPass::Initialize(ILightingProcess *process) {
    this->process = process;

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return COM::kError;
    }

    // Compile shader
    shaderAsset = host->CompileAssetAsync<Data::ShaderAsset>("Runtime/Engine/Shaders/DeferredDirectionalLight.dsf"_path);

    // OK
    return COM::kOK;
}

COM::Result DeferredDirectionalLightPass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result DeferredDirectionalLightPass::Build(PassBuilder builder, double delta) {
    // Get packet
    auto packet = process->GetPacket();
    const auto &resources = process->GetResources();

    // Get buffer
    auto buffer = packet->batch.GetPacket<SceneDirectionalLightPacketBuffer>();

    // Camera data
    struct CameraData {
        Math::Matrix4 view;
        Math::Matrix4 viewInv;
        Math::Matrix4 projection;
        Math::Matrix4 projectionInv;
    } cameraData = {
            packet->view.Transposed(),
            packet->view.Transposed().Inversed(),
            packet->projection.Transposed(),
            packet->projection.Transposed().Inversed()
    };

    // Instance data
    struct InstanceData {
        Math::Matrix4 shadowViewProjection;
        Math::Vector4 attributes;
        Math::Vector4 param0;
        Math::Vector4 param1;
    };

    // Batch data
    struct BatchData {
        InstanceData instances[512];
    } batchData;

    // Bind
    builder.BindPacked("Data.camera"_id, &cameraData);
    builder.BindRenderTarget(0, process->GetResources().direct, GHA::TextureViews::none, GHA::BlendTargets::transperancy);
    builder.SetViewport(GHA::Rect(resources.direct->size));
    builder.SetScissor(GHA::Rect(resources.direct->size));
    builder.BindShader(shaderAsset);
    builder.SetRasterizerState(GHA::Rasterizers::noCull);

    // Collect
    auto &packets = buffer->GetPackets();

    // Current instance
    UInt32 instanceCount = 0;

    // Process directional lights
    for (USize i = 0; i < packets.Size(); i++) {
        auto &light = packets[i];

        // Fill batch.
        batchData.instances[instanceCount].attributes.x = 0;
        batchData.instances[instanceCount].attributes.y = 0;
        batchData.instances[instanceCount].param0 = Math::Vector4(light.color, light.intensity);
        batchData.instances[instanceCount].param1 = Math::Vector4(light.direction, 0);

        // Increment
        instanceCount++;

        // Flush?
        if (instanceCount >= 512 || (i == packets.Size() - 1)) {
            builder.BindPacked("Data.batch"_id, &batchData, sizeof(InstanceData) * instanceCount);
            builder.Draw(3, instanceCount);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result DeferredDirectionalLightPass::Configure() {
    // OK
    return COM::kOK;
}
