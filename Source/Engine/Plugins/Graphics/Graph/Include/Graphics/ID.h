//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 12/26/2017.
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