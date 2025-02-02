//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <Core/Unique.h>
#include <GHA/TextureDesc.h>
#include <GHA/SamplerDesc.h>
#include <GHA/VertexBufferDesc.h>
#include <GHA/StructuredBufferDesc.h>
#include <GHA/TextureBufferDesc.h>
#include <GHA/IndexBufferDesc.h>
#include <GHA/ID.h>

namespace Djinn::Graphics {
    /// Wrapper
    template<typename ID, typename DESC>
    struct ResourceWrapper {
    public:
        using Type = ID;

        ResourceWrapper() = default;
        ResourceWrapper(ID uid, const DESC& desc) : uid(uid), desc(desc) {

        }

        /// Implicit resource id
        /// \return
        operator ID() const {
            return uid;
        }

        /// Implicit resource id
        /// \return
        operator GHA::ResourceID() const {
            return uid;
        }

        /// Get unique id
        /// \return
        ID GetUID() const {
            return uid;
        }

        /// Get description
        /// \return
        const DESC& GetDesc() const {
            return desc;
        }

        /// Accessor
        /// \return
        DESC* operator->() {
            return &desc;
        }

        /// Accessor
        /// \return
        const DESC* operator->() const {
            return &desc;
        }

    private:
        ID uid;
        DESC desc;
    };

    /// Wrappers
    using Texture = ResourceWrapper<GHA::TextureID, GHA::TextureDesc>;
    using StructuredBuffer = ResourceWrapper<GHA::StructuredBufferID, GHA::StructuredBufferDesc>;
    using TextureBuffer = ResourceWrapper<GHA::TextureBufferID, GHA::TextureBufferDesc>;
    using VertexBuffer = ResourceWrapper<GHA::VertexBufferID, GHA::VertexBufferDesc>;
    using IndexBuffer = ResourceWrapper<GHA::IndexBufferID, GHA::IndexBufferDesc>;
    using Sampler = ResourceWrapper<GHA::SamplerID, GHA::SamplerDesc>;

    /// Mesh wrapper
    struct Mesh {
        Mesh() = default;
        Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer) : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {

        }

        VertexBuffer vertexBuffer;
        IndexBuffer indexBuffer;
    };
}