//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/Array.h>
#include <Data/Mesh/BaseIndex.h>
#include <Data/Mesh/SkeletalVertex.h>
#include "IMeshAsset.h"
#include <GHA/IStreamable.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH SkeletalMeshAsset : public IMeshAsset {
    public:
        DJINN_COM_CLASS();

        SkeletalMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::ClassID GetLayoutClassID() override;
        virtual USize GetVertexStride() override;
        virtual GHA::IndexType GetIndexType() override;
        virtual GHA::IStreamable *GetVertexStream() override;
        virtual GHA::IStreamable *GetIndexStream() override;

        /// Indices
        class IndexStream : public GHA::IStreamable {
        public:
            virtual COM::Result Enumerate(USize &size, void **blob) override;

            Core::Array<BaseIndex> indices;
        } indexStream;

        /// Vertices
        class VertexStream : public GHA::IStreamable {
        public:
            virtual COM::Result Enumerate(USize &size, void **blob) override;

            Core::Array<SkeletalVertex> vertices;
        } vertexStream;
    };
}