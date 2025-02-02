//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Data/Mesh/BaseVertex.h>
#include <Core/Array.h>
#include <Data/Mesh/BaseIndex.h>
#include "IMeshAsset.h"
#include <GHA/IStreamable.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH BaseMeshAsset : public IMeshAsset {
    public:
        DJINN_COM_CLASS();

        BaseMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual COM::ClassID GetLayoutClassID() override;
        virtual GHA::IStreamable *GetVertexStream() override;
        virtual GHA::IStreamable *GetIndexStream() override;
        virtual GHA::IndexType GetIndexType() override;
        virtual USize GetVertexStride() override;

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

            Core::Array<BaseVertex> vertices;
        } vertexStream;
    };
}