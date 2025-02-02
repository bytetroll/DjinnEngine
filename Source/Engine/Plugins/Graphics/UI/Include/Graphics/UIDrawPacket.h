//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Math/Vector2.h>
#include <Graphics/PacketBuffer.h>
#include <Core/Memory.h>
#include <Math/Matrix4.h>
#include <Platform/Mutex.h>
#include <Platform/ConcurrentStorageBuffer.h>
#include "UIBrushPacket.h"
#include "UIGeometryPacket.h"

namespace Djinn::Data {
    class IFontAsset;
}

namespace Djinn::Graphics {
    struct UIDrawPacket {
        /// Transformation
        Math::Matrix4 transform;

        /// Viewport
        Math::Vector4 viewport = {0, 0, 1e6, 1e6};

        /// Geometry to draw
        UIGeometryPacket geometry;

        /// Brush to be applied
        UIBrushPacket brush;
    };

    /// Buffer helper
    class UIDrawPacketBuffer : public IPacket {
    public:
        DJINN_COM_CLASSID();

        /// Add to this buffer
        void Add(UIDrawPacket packet) {
            PreparePacket(packet);
            buffer.Add(packet);
        }

        /// Flush this buffer
        void Flush() override {
            buffer.Flush();
            vertexStorage.Clear();
            indexStorage.Clear();
            glyphStorage.Clear();
        }

        /// Get buffer
        /// \return
        PacketBuffer<UIDrawPacket> &GetBuffer() {
            return buffer;
        }

        /// Pop a temporary buffer
        /// \return
        Core::Buffer<UIDrawPacket> *PopBuffer() {
            return buffer.PopBuffer();
        }

        /// Push a temporary buffer
        /// \param buffer
        void PushBuffer(Core::Buffer<UIDrawPacket> *_buffer) {
            for (auto &packet : *_buffer) {
                PreparePacket(packet);
            }
            buffer.PushBuffer(_buffer);
        }

    private:
        void PreparePacket(UIDrawPacket &packet) {
            // Provide custom storage for custom geometry data
            if (packet.geometry.type == UIGeometryType::eCustom) {
                packet.geometry.custom.vertices = Core::ArrayView<UIGeometryVertex>(
                        vertexStorage.Add(packet.geometry.custom.vertices.Size(), packet.geometry.custom.vertices.Ptr()),
                        packet.geometry.custom.vertices.Size()
                );
                packet.geometry.custom.indices = Core::ArrayView<UIGeometryIndex>(
                        indexStorage.Add(packet.geometry.custom.indices.Size(), packet.geometry.custom.indices.Ptr()),
                        packet.geometry.custom.indices.Size()
                );
            }

            // Provide custom storage for line geometry data
            if (packet.geometry.type == UIGeometryType::eLine) {
                packet.geometry.line.vertices = Core::ArrayView<UIGeometryVertex>(
                        vertexStorage.Add(packet.geometry.line.vertices.Size(), packet.geometry.line.vertices.Ptr()),
                        packet.geometry.line.vertices.Size()
                );
            }

            // Provide custom storage for font geometry data
            if (packet.geometry.type == UIGeometryType::eFont) {
                packet.geometry.font.glyphs = Core::ArrayView<Data::GlyphRender>(
                        glyphStorage.Add(packet.geometry.font.glyphs.Size(), packet.geometry.font.glyphs.Ptr()),
                        packet.geometry.font.glyphs.Size()
                );
            }
        }

    private:
        PacketBuffer<UIDrawPacket> buffer;
        Platform::ConcurrentStorageBuffer<UIGeometryVertex, 10'000> vertexStorage;
        Platform::ConcurrentStorageBuffer<UIGeometryIndex, 10'000> indexStorage;
        Platform::ConcurrentStorageBuffer<Data::GlyphRender, 10'000> glyphStorage;
    };
}