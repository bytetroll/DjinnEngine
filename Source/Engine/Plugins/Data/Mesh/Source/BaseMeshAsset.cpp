//
// Created by Dev on 10/23/2017.
//
#include <Data/BaseMeshAsset.h>
#include <Data/BaseMaterialLayout.h>

DJINN_NS2(Data);

BaseMeshAsset::BaseMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshAsset(registry, outer, host) {

}


GHA::IStreamable *BaseMeshAsset::GetVertexStream() {
    return &vertexStream;
}

GHA::IStreamable *BaseMeshAsset::GetIndexStream() {
    return &indexStream;
}

GHA::IndexType BaseMeshAsset::GetIndexType() {
    return GHA::IndexType::eU32;
}

USize BaseMeshAsset::GetVertexStride() {
    return sizeof(BaseVertex);
}

COM::ClassID BaseMeshAsset::GetLayoutClassID() {
    return BaseMaterialLayout::kCID;
}

COM::Result BaseMeshAsset::IndexStream::Enumerate(USize &size, void **blob) {
    size = indices.ByteSize();
    *blob = indices.Ptr();
    return COM::kOK;
}

COM::Result BaseMeshAsset::VertexStream::Enumerate(USize &size, void **blob) {
    size = vertices.ByteSize();
    *blob = vertices.Ptr();
    return COM::kOK;
}
