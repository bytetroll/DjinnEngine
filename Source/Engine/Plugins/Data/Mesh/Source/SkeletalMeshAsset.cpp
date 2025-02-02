//
// Created by Dev on 10/23/2017.
//
#include <Data/SkeletalMeshAsset.h>

DJINN_NS2(Data);

SkeletalMeshAsset::SkeletalMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshAsset(registry, outer, host) {

}


USize SkeletalMeshAsset::GetVertexStride() {
    return sizeof(SkeletalVertex);
}

GHA::IndexType SkeletalMeshAsset::GetIndexType() {
    return GHA::IndexType::eU32;
}

GHA::IStreamable *SkeletalMeshAsset::GetVertexStream() {
    return &vertexStream;
}

GHA::IStreamable *SkeletalMeshAsset::GetIndexStream() {
    return &indexStream;
}

COM::ClassID SkeletalMeshAsset::GetLayoutClassID() {
    DJINN_THROW std::exception();
}

COM::Result SkeletalMeshAsset::IndexStream::Enumerate(USize &size, void **blob) {
    size = indices.ByteSize();
    *blob = indices.Ptr();
    return COM::kOK;
}

COM::Result SkeletalMeshAsset::VertexStream::Enumerate(USize &size, void **blob) {
    size = vertices.ByteSize();
    *blob = vertices.Ptr();
    return COM::kOK;
}
