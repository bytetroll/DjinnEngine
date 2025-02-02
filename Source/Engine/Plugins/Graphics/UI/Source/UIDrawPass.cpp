//
// Created by Dev on 2/26/2018.
//

#include <Graphics/UIDrawPass.h>
#include <Graphics/IUIProcess.h>
#include <Graphics/IOutput.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/UIDrawPacket.h>
#include <Graphics/UILayerPacket.h>
#include <Graphics/UIPacket.h>
#include <Data/GlyphConfig.h>
#include <Math/Matrix4.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/ShaderConfiguration.h>
#include <Data/ShaderAsset.h>
#include <Data/IFontAsset.h>
#include <Data/IAssetHost.h>

DJINN_NS2(Graphics);

COM::Result UIDrawPass::Initialize(IUIProcess *process) {
    this->process = process;

    // Get asset worldHost
    Data::IAssetHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("UI draw pass expected an asset worldHost to be installed");
        return COM::kError;
    }

    // Compile shader
    shaderAsset = host->CompileAssetAsync<Data::ShaderAsset>("Runtime/Engine/Shaders/UIDraw.dsf"_path);

    // OK
    return COM::kOK;
}

COM::Result UIDrawPass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result UIDrawPass::Build(PassBuilder builder, double delta) {
    // Flush previous.
    vertices.Clear();
    vertices.Commit();
    indices.Clear();
    indices.Commit();

    // Get packet
    auto packet = process->GetPacket();
    if (!packet->output) {
        // TODO: Logging of sorts?
        return COM::kOK;
    }

    // Get target
    auto target = packet->output->GetTexture(builder);

    // Config
    constexpr UInt32 instanceLimit = 450;

    // Camera data
    struct CameraData {
        Math::Matrix4 viewProjection;
    } cameraData = {
            (Math::Matrix4::Scale({2.0f / target->size.width, 2.0f / target->size.height, 1}) * Math::Matrix4::Translate({-1, -1, 0})).Transposed()
    };

    // Instance data.
    struct InstanceData {
        Math::Matrix4 transform;
        Math::Vector4 viewport;
        Math::Vector4 attributes;
        Math::Vector4 param0;
        Math::Vector4 param1;
        Math::Vector4 param2;
    };

    // Batch data
    struct BatchData {
        InstanceData instances[instanceLimit];
    } batchData;

    // Bind
    builder.BindPacked("Data.camera"_id, &cameraData);
    builder.BindRenderTarget(0, target, GHA::TextureViews::none, GHA::BlendTargets::transperancy);
    builder.BindDepthRenderTarget(process->GetDepthTexture(), GHA::TextureViews::none);
    builder.BindVertexBuffer(0, builder.AddVertexArray(vertices, true));
    builder.BindIndexBuffer(builder.AddIndexArray(indices, true));
    builder.SetViewport(GHA::Rect(target->size));
    builder.SetScissor(GHA::Rect(target->size));
    builder.SetRasterizerState(GHA::Rasterizers::noCull);
    builder.BindShader(shaderAsset);
    builder.SetSampleMode(GHA::SampleMode::e4);

    // Get layer buffer
    auto& layers = packet->batch.GetPacket<UILayerPacketBuffer>()->GetPackets();

    // Sort by depth
    // Render lowest first
    layers.Sort([](auto a, auto b) { return a->depth < b->depth; });

    // Indexes
    UInt32 baseIndex = 0;

    // Process all layers
    for (auto layer : layers) {
        textureLUT.Clear();

        // Layers do not inherit depth
        builder.ClearDepthStencil(process->GetDepthTexture());

        // Get buffer
        auto buffer = layer->batch.GetPacket<UIDrawPacketBuffer>();

        // Collect all packets
        auto &packets = buffer->GetBuffer().GetPackets();

        // Sort
        packets.Sort([](UIDrawPacket &a, UIDrawPacket &b) { return a.transform.w.z < b.transform.w.z; });

        // Indexes
        UInt32 instanceCount = 0;
        UInt32 textureCount = 0;

        // Bind helper
        auto Bind = [&](Texture texture, const GHA::SamplerDesc &desc) {
            auto it = textureLUT.Find(texture);
            if (it == textureLUT.End()) {
                textureLUT.Add(texture, textureCount);
                builder.BindTexture("Resources.textureMap"_id, static_cast<UInt16>(textureCount++), texture, GHA::TextureViews::none, desc);
                return textureCount - 1;
            }
            return (*it).second;
        };

        // Process all packets
        for (USize packetIndex = 0; packetIndex < packets.Size(); packetIndex++) {
            auto &packet = packets[packetIndex];

            // Instance
            packet.transform.w.z = 1.0f - packet.transform.w.z / 10'000.0f;
            batchData.instances[instanceCount].transform = packet.transform.Transposed();
            batchData.instances[instanceCount].viewport = packet.viewport;

            // Brush
            switch (packet.brush.type) {
                case UIBrushType::eNone:
                    continue;
                case UIBrushType::eSolid: {
                    batchData.instances[instanceCount].attributes.x = 0;
                    batchData.instances[instanceCount].param0 = packet.brush.solid.color;
                    break;
                }
                case UIBrushType::eGradient: {
                    batchData.instances[instanceCount].attributes.x = 1;
                    batchData.instances[instanceCount].param0 = Math::Vector4(packet.brush.gradient.begin, packet.brush.gradient.end);
                    batchData.instances[instanceCount].param1 = packet.brush.gradient.beginColor;
                    batchData.instances[instanceCount].param2 = packet.brush.gradient.endColor;
                    break;
                }
                case UIBrushType::eGHATexture: {
                    batchData.instances[instanceCount].attributes.x = 2;
                    batchData.instances[instanceCount].attributes.y = Bind(builder.AddTexture(packet.brush.ghaTexture.texture), packet.brush.ghaTexture.sampler);
                    batchData.instances[instanceCount].param0 = packet.brush.texture.color;
                    break;
                }
                case UIBrushType::eTexture: {
                    batchData.instances[instanceCount].attributes.x = 2;
                    batchData.instances[instanceCount].attributes.y = Bind(builder.AddTexture(packet.brush.texture.asset), packet.brush.texture.sampler);
                    batchData.instances[instanceCount].param0 = packet.brush.texture.color;
                    break;
                }
                case UIBrushType::eOutputTexture: {
                    batchData.instances[instanceCount].attributes.x = 2;
                    batchData.instances[instanceCount].attributes.y = textureCount;
                    batchData.instances[instanceCount].param0 = packet.brush.texture.color;

                    // Bind texture
                    builder.BindTexture("Resources.textureMap"_id, static_cast<UInt16>(textureCount++), packet.brush.outputTexture.output->GetTexture(builder), GHA::TextureViews::none, packet.brush.texture.sampler);
                    break;
                }
                case UIBrushType::eSDF: {
                    batchData.instances[instanceCount].attributes.x = 3;
                    batchData.instances[instanceCount].param0 = {packet.brush.sdf.smoothing, 0, 0, 0};
                    batchData.instances[instanceCount].param1 = packet.brush.sdf.color;
                    break;
                }
            }

            // Vertex template
            DrawVertex vertex;
            vertex.index = instanceCount;

            // Generate geometry
            switch (packet.geometry.type) {
                case UIGeometryType::eNone:
                    continue;
                case UIGeometryType::eBox: {
                    auto vertexOffset = static_cast<UInt32>(vertices.Size());

                    // 0
                    vertex.position = Math::Vector2(0, 0);
                    vertex.texcoord = Math::Vector2(0, 0);
                    vertices.Add(vertex);

                    // 1
                    vertex.position = Math::Vector2(packet.geometry.box.size.x, 0);
                    vertex.texcoord = Math::Vector2(1, 0);
                    vertices.Add(vertex);

                    // 2
                    vertex.position = packet.geometry.box.size;
                    vertex.texcoord = Math::Vector2(1, 1);
                    vertices.Add(vertex);

                    // 3
                    vertex.position = Math::Vector2(0, packet.geometry.box.size.y);
                    vertex.texcoord = Math::Vector2(0, 1);
                    vertices.Add(vertex);

                    // Indices
                    indices.Add(vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, vertexOffset + 0, vertexOffset + 2, vertexOffset + 3);
                    break;
                }
                case UIGeometryType::eMarginBox: {
                    auto vertexOffset = static_cast<UInt32>(vertices.Size());

                    /*  0---8-------9---1
                     *  | A |  B    | C |
                     * 15---4-------5---10
                     *  | H |    I  | D |
                     * 14---7-------6---11
                     *  | G |   F   | E |
                     *  3--13------12---2
                     * */

                    // Indices
                    indices.Add(
                            // A
                            vertexOffset + 0, vertexOffset + 8, vertexOffset + 4, vertexOffset + 0, vertexOffset + 4, vertexOffset + 15,

                            // B
                            vertexOffset + 8, vertexOffset + 9, vertexOffset + 5, vertexOffset + 8, vertexOffset + 5, vertexOffset + 4,

                            // C
                            vertexOffset + 9, vertexOffset + 1, vertexOffset + 10, vertexOffset + 9, vertexOffset + 10, vertexOffset + 5,

                            // D
                            vertexOffset + 5, vertexOffset + 10, vertexOffset + 11, vertexOffset + 5, vertexOffset + 11, vertexOffset + 6,

                            // E
                            vertexOffset + 6, vertexOffset + 11, vertexOffset + 2, vertexOffset + 6, vertexOffset + 2, vertexOffset + 12,

                            // F
                            vertexOffset + 7, vertexOffset + 6, vertexOffset + 12, vertexOffset + 7, vertexOffset + 12, vertexOffset + 13,

                            // G
                            vertexOffset + 14, vertexOffset + 7, vertexOffset + 13, vertexOffset + 14, vertexOffset + 13, vertexOffset + 3,

                            // H
                            vertexOffset + 15, vertexOffset + 4, vertexOffset + 7, vertexOffset + 15, vertexOffset + 7, vertexOffset + 14,

                            // I
                            vertexOffset + 4, vertexOffset + 5, vertexOffset + 6, vertexOffset + 4, vertexOffset + 6, vertexOffset + 7
                    );

                    // 0
                    vertex.position = Math::Vector2(0, 0);
                    vertex.texcoord = Math::Vector2(0, 0);
                    vertices.Add(vertex);

                    // 1
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x, 0);
                    vertex.texcoord = Math::Vector2(1, 0);
                    vertices.Add(vertex);

                    // 2
                    vertex.position = packet.geometry.marginBox.size;
                    vertex.texcoord = Math::Vector2(1, 1);
                    vertices.Add(vertex);

                    // 3
                    vertex.position = Math::Vector2(0, packet.geometry.marginBox.size.y);
                    vertex.texcoord = Math::Vector2(0, 1);
                    vertices.Add(vertex);

                    // 4
                    vertex.position = packet.geometry.marginBox.margin;
                    vertex.texcoord = Math::Vector2(1.0f / 3.0f, 1.0f / 3.0f);
                    vertices.Add(vertex);

                    // 5
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x, 0) + packet.geometry.marginBox.margin * Math::Vector2(-1, 1);
                    vertex.texcoord = Math::Vector2(2.0f / 3.0f, 1.0f / 3.0f);
                    vertices.Add(vertex);

                    // 6
                    vertex.position = packet.geometry.marginBox.size - packet.geometry.marginBox.margin;
                    vertex.texcoord = Math::Vector2(2.0f / 3.0f, 2.0f / 3.0f);
                    vertices.Add(vertex);

                    // 7
                    vertex.position = Math::Vector2(0, packet.geometry.marginBox.size.y) + packet.geometry.marginBox.margin * Math::Vector2(1, -1);
                    vertex.texcoord = Math::Vector2(1.0f / 3.0f, 2.0f / 3.0f);
                    vertices.Add(vertex);

                    // 8
                    vertex.position = Math::Vector2(packet.geometry.marginBox.margin.x, 0);
                    vertex.texcoord = Math::Vector2(1.0f / 3.0f, 0);
                    vertices.Add(vertex);

                    // 9
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x - packet.geometry.marginBox.margin.x, 0);
                    vertex.texcoord = Math::Vector2(2.0f / 3.0f, 0);
                    vertices.Add(vertex);

                    // 10
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x, packet.geometry.marginBox.margin.y);
                    vertex.texcoord = Math::Vector2(1, 1.0f / 3.0f);
                    vertices.Add(vertex);

                    // 11
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x, packet.geometry.marginBox.size.y - packet.geometry.marginBox.margin.y);
                    vertex.texcoord = Math::Vector2(1, 2.0f / 3.0f);
                    vertices.Add(vertex);

                    // 12
                    vertex.position = Math::Vector2(packet.geometry.marginBox.size.x - packet.geometry.marginBox.margin.x, packet.geometry.marginBox.size.y);
                    vertex.texcoord = Math::Vector2(2.0f / 3.0f, 1);
                    vertices.Add(vertex);

                    // 13
                    vertex.position = Math::Vector2(packet.geometry.marginBox.margin.x, packet.geometry.marginBox.size.y);
                    vertex.texcoord = Math::Vector2(1.0f / 3.0f, 1);
                    vertices.Add(vertex);

                    // 14
                    vertex.position = Math::Vector2(0, packet.geometry.marginBox.size.y - packet.geometry.marginBox.margin.y);
                    vertex.texcoord = Math::Vector2(0, 2.0f / 3.0f);
                    vertices.Add(vertex);

                    // 15
                    vertex.position = Math::Vector2(0, packet.geometry.marginBox.margin.y);
                    vertex.texcoord = Math::Vector2(0, 1.0f / 3.0f);
                    vertices.Add(vertex);
                    break;
                }
                case UIGeometryType::eCircle: {
                    auto vertexOffset = static_cast<UInt32>(vertices.Size());

                    /*vertex.position = Math::Vector2(0, 0);
                    vertex.

                    for (UInt32 i = 0; i < packet.geometry.circle.segments; i++) {

                    }*/
                    break;
                }
                case UIGeometryType::eCustom: {
                    auto vertexOffset = static_cast<UInt32>(vertices.Size());

                    // Append indices
                    for (UInt32 i = 0; i < packet.geometry.custom.indices.Size(); i++) {
                        indices.Add(vertexOffset + packet.geometry.custom.indices[i]);
                    }

                    // Append vertices
                    for (UInt32 i = 0; i < packet.geometry.custom.vertices.Size(); i++) {
                        vertex.position = packet.geometry.custom.vertices[i].position;
                        vertex.texcoord = packet.geometry.custom.vertices[i].texcoord;
                        vertices.Add(vertex);
                    }
                    break;
                }
                case UIGeometryType::eFont: {
                    if (!packet.geometry.font.font.Get()) {
                        break;
                    }

                    batchData.instances[instanceCount].attributes.y = Bind(builder.AddTexture(packet.geometry.font.font->GetTexture()), GHA::Samplers::noMip);

                    // Process glyphs
                    for (USize i = 0; i < packet.geometry.font.glyphs.Size(); i++) {
                        auto &glyph = packet.geometry.font.glyphs[i];

                        // Determine pixel size
                        auto size = (glyph.config.end - glyph.config.begin);

                        // Base vertex
                        auto baseVertex = static_cast<UInt32>(vertices.Size());

                        // Glyph offset
                        auto glyphOffset = glyph.config.offset * Math::Vector2(1, -1) + Math::Vector2(0, packet.geometry.font.font->GetGlyphSize());

                        // 0
                        vertex.position = glyphOffset * glyph.size + glyph.offset;
                        vertex.texcoord = glyph.config.uvBegin;
                        vertices.Add(vertex);

                        // 1
                        vertex.position = (glyphOffset + size * Math::Vector2(1, 0)) * glyph.size + glyph.offset;
                        vertex.texcoord = Math::Vector2(glyph.config.uvEnd.x, glyph.config.uvBegin.y);
                        vertices.Add(vertex);

                        // 2
                        vertex.position = (glyphOffset + size) * glyph.size + glyph.offset;
                        vertex.texcoord = glyph.config.uvEnd;
                        vertices.Add(vertex);

                        // 3
                        vertex.position = (glyphOffset + size * Math::Vector2(0, 1)) * glyph.size + glyph.offset;
                        vertex.texcoord = Math::Vector2(glyph.config.uvBegin.x, glyph.config.uvEnd.y);
                        vertices.Add(vertex);

                        // Indices
                        indices.Add(
                                baseVertex + 0, baseVertex + 1, baseVertex + 2, baseVertex + 0, baseVertex + 2, baseVertex + 3
                        );
                    }
                    break;
                }
                case UIGeometryType::eLine: {
                    // ...
                    if (packet.geometry.line.connected) {
                        for (UInt32 i = 0; i < packet.geometry.line.vertices.Size() - 1; i++) {
                            auto vertexOffset = static_cast<UInt32>(vertices.Size());

                            auto d = packet.geometry.line.vertices[i + 1].position - packet.geometry.line.vertices[i].position;
                            auto r = Math::Vector2(-d.y, d.x).Normalized();

                            vertex.position = packet.geometry.line.vertices[i].position - r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i + 1].position - r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i + 1].position + r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i].position + r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            indices.Add(vertexOffset + 0, vertexOffset + 1, vertexOffset + 3, vertexOffset + 1, vertexOffset + 2, vertexOffset + 3);
                        }
                    } else {
                        for (UInt32 i = 0; i < packet.geometry.line.vertices.Size(); i += 2) {
                            auto vertexOffset = static_cast<UInt32>(vertices.Size());

                            auto d = packet.geometry.line.vertices[i + 1].position - packet.geometry.line.vertices[i].position;
                            auto r = Math::Vector2(-d.y, d.x).Normalized();

                            vertex.position = packet.geometry.line.vertices[i].position - r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i + 1].position - r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i + 1].position + r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            vertex.position = packet.geometry.line.vertices[i].position + r * packet.geometry.line.width * 0.5f;
                            vertices.Add(vertex);

                            indices.Add(vertexOffset + 0, vertexOffset + 1, vertexOffset + 3, vertexOffset + 1, vertexOffset + 2, vertexOffset + 3);
                        }
                    }
                    break;
                }
            }

            // ...
            instanceCount++;

            // Draw
            if (packetIndex == packets.Size() - 1 || instanceCount >= instanceLimit || textureCount >= 64) {
                builder.BindPacked("Data.batch"_id, &batchData, sizeof(InstanceData) * instanceCount);

                auto indexCount = static_cast<UInt32>(indices.Size()) - baseIndex;
                if (indexCount > 0) {
                    builder.SetTopology(GHA::Topology::eTriangleList);
                    builder.DrawIndexed(indexCount, 1, 0, baseIndex);
                }
                baseIndex = static_cast<UInt32>(indices.Size());
                instanceCount = 0;
                textureCount = 0;
                textureLUT.Clear();
            }
        }
    }

    // OK
    return COM::kOK;
}

COM::Result UIDrawPass::Configure() {
    // OK
    return COM::kOK;
}

