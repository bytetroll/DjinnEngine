//
// Created by Dev on 10/24/2017.
//

#include <Data/MeshCompiler.h>
#include <Data/MeshSourceFormat.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>
#include <Platform/FileStream.h>
#include <Platform/FileSystem.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Data/MeshCollectionAsset.h>
#include <Data/SkeletalMeshAsset.h>
#include <Data/BaseMeshAsset.h>
#include <Core/Timer.h>
#include <Base/IFileSystem.h>
#include <Base/FileHandle.h>

DJINN_NS2(Data);

namespace {
    class AssimpIOStream : public Assimp::IOStream {
    public:
        AssimpIOStream(const MeshConfiguration *info, Core::IStream *stream) : info(info), stream(stream) {

        }

        ~AssimpIOStream() {

        }

        virtual size_t Read(void *pvBuffer, size_t pSize, size_t pCount) override {
            return stream->Read(pvBuffer, pSize * pCount);
        }

        virtual size_t Write(const void *pvBuffer, size_t pSize, size_t pCount) override {
            return stream->Write(pvBuffer, pSize * pCount);
        }

        virtual aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override {
            switch (pOrigin) {
                case aiOrigin_SET:
                    stream->SetPosition(pOffset);
                    break;
                case aiOrigin_CUR:
                    stream->SetPosition(stream->GetPosition() + pOffset);
                    break;
                case aiOrigin_END:
                    stream->SetPosition(stream->GetSize() - pOffset);
                    break;
                default:
                    // Unknown origin
                    return AI_FAILURE;
            }

            // All good
            return AI_SUCCESS;
        }

        virtual size_t Tell() const override {
            return stream->GetPosition();
        }

        virtual size_t FileSize() const override {
            return stream->GetSize();
        }

        virtual void Flush() override {
            stream->Flush();
        }

        Core::IStream *stream;
        const MeshConfiguration *info;
    };

    class AssimpIOSystem : public Assimp::IOSystem {
    public:
        AssimpIOSystem(Base::IFileSystem* fs, const MeshConfiguration *info, Core::IStream *stream) : fs(fs), info(info), stream(stream) {

        }

        virtual bool Exists(const char *pFile) const override {
            if (pFile[0] == '&') {
                return true;
            } else {
                return false;
            }
        }

        virtual char getOsSeparator() const override {
            return '/';
        }

        virtual Assimp::IOStream *Open(const char *pFile, const char *) override {
            if (pFile[0] == '&') {
                return new AssimpIOStream(info, stream);
            } else {
                /*Core::String path = pFile;

                Base::FileHandle handle;
                if (!fs->OpenStream(Base::ToPath(pFile), Platform::FileMode::eRead, &handle)) {
                    if (!fs->OpenStream(Base::ToPath(Platform::FileSystem::CombinePaths(info->includePath, path).AsCharArray()), Platform::FileMode::eRead, &handle)) {
                        return nullptr;
                    }
                }

                handle.ManualRelease();
                return new AssimpIOStream(info, handle.GetStream());*/
                Core::String path = pFile;
                if (!Platform::FileSystem::Exists(path)) {
                    path = Platform::FileSystem::CombinePaths(info->includePath, path);
                }
                Platform::FileStream *str = new Platform::FileStream(Platform::FileMode::eRead, path);
                if (!str->IsGood()) {
                    delete str;
                    return nullptr;
                }
                return new AssimpIOStream(info, str);
            }
        }

        virtual void Close(Assimp::IOStream *pFile) override {
            if (static_cast<AssimpIOStream *>(pFile)->stream != stream) {
                delete static_cast<AssimpIOStream *>(pFile)->stream;
            }
        }

        Base::IFileSystem* fs;
        Core::IStream *stream;
        const MeshConfiguration *info;
    };
}

MeshCompiler::MeshCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshCompiler(registry, outer, host) {

}

COM::Result MeshCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = static_cast<MeshConfiguration *>(_config);

    // Diagnostic
    Core::Timer timer;

    // IO Wrapper
    AssimpIOSystem *ioSystem = new AssimpIOSystem(fs, config, stream);

    // Prepare importer
    Assimp::Importer importer;
    {
        importer.SetIOHandler(ioSystem);
    }

    // Assimp determines format by extension
    const char *fileFormat;
    switch (config->format) {
        case MeshSourceFormat::eObj:
            fileFormat = "&.obj";
            break;
        default:
            ErrorLog(this).Write("Invalid mesh source format");
            return COM::kError;
    }

    // Determine flags
    UInt32 flags = 0;

    // Read file
    const aiScene *scene = importer.ReadFile(fileFormat, flags);

    // Perform pre-processing
    importer.ApplyPostProcessing(aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipWindingOrder);

    // Create asset
    MeshCollectionAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    // Populate
    for (USize iMesh = 0; iMesh < scene->mNumMeshes; iMesh++) {
        const aiMesh *mesh = scene->mMeshes[iMesh];

        // Skeletal?
        if (mesh->HasBones()) {
            // Create mesh asset
            SkeletalMeshAsset *meshAsset = nullptr;
            if (!registry->CreateClass(nullptr, &meshAsset)) {
                return COM::kError;
            }

            // Preallocate
            meshAsset->indexStream.indices.Resize(mesh->mNumFaces * 3);
            meshAsset->vertexStream.vertices.Resize(mesh->mNumVertices);

            // Fill indices
            for (USize iFace = 0; iFace < mesh->mNumFaces; iFace++) {
                for (USize iIndex = 0; iIndex < mesh->mFaces[iFace].mNumIndices; iIndex++) {
                    meshAsset->indexStream.indices[iFace * 3 + iIndex] = mesh->mFaces[iFace].mIndices[iIndex];
                }
            }

            // Fill vertices
            for (USize iVertex = 0; iVertex < mesh->mNumVertices; iVertex++) {
                meshAsset->vertexStream.vertices[iVertex].position = Math::Vector3(mesh->mVertices[iVertex][0], mesh->mVertices[iVertex][1], mesh->mVertices[iVertex][2]);
                if (mesh->mNormals) {
                    meshAsset->vertexStream.vertices[iVertex].normal = Math::Vector3(mesh->mNormals[iVertex][0], mesh->mNormals[iVertex][1], mesh->mNormals[iVertex][2]);
                }
                if (mesh->mTangents) {
                    meshAsset->vertexStream.vertices[iVertex].tangent = Math::Vector3(mesh->mTangents[iVertex][0], mesh->mTangents[iVertex][1], mesh->mTangents[iVertex][2]);
                }
                if (*mesh->mTextureCoords && mesh->mTextureCoords[0]) {
                    meshAsset->vertexStream.vertices[iVertex].texcoord = Math::Vector2(mesh->mTextureCoords[0][iVertex].x, mesh->mTextureCoords[0][iVertex].y);
                }
                //meshAsset->vertices[iVertex].boneIndices = Math::Vector4(mesh->);
            }

            // ...
            asset->meshes.Add(meshAsset);
        } else {
            // Create mesh asset
            BaseMeshAsset *meshAsset = nullptr;
            if (!registry->CreateClass(nullptr, &meshAsset)) {
                return COM::kError;
            }

            // Preallocate
            meshAsset->indexStream.indices.Resize(mesh->mNumFaces * 3);
            meshAsset->vertexStream.vertices.Resize(mesh->mNumVertices);

            // Fill indices
            for (USize iFace = 0; iFace < mesh->mNumFaces; iFace++) {
                for (USize iIndex = 0; iIndex < mesh->mFaces[iFace].mNumIndices; iIndex++) {
                    BaseIndex &a = meshAsset->indexStream.indices[iFace * 3 + iIndex];
                    unsigned int &B = mesh->mFaces[iFace].mIndices[iIndex];
                    a = B;
                    meshAsset->indexStream.indices[iFace * 3 + iIndex] = mesh->mFaces[iFace].mIndices[iIndex];
                }
            }

            // Fill vertices
            for (USize iVertex = 0; iVertex < mesh->mNumVertices; iVertex++) {
                meshAsset->vertexStream.vertices[iVertex].position = Math::Vector3(mesh->mVertices[iVertex][0], mesh->mVertices[iVertex][1], mesh->mVertices[iVertex][2]);
                if (mesh->mNormals) {
                    meshAsset->vertexStream.vertices[iVertex].normal = Math::Vector3(mesh->mNormals[iVertex][0], mesh->mNormals[iVertex][1], mesh->mNormals[iVertex][2]);
                }
                if (mesh->mTangents) {
                    meshAsset->vertexStream.vertices[iVertex].tangent = Math::Vector3(mesh->mTangents[iVertex][0], mesh->mTangents[iVertex][1], mesh->mTangents[iVertex][2]);
                }
                if (*mesh->mTextureCoords && mesh->mTextureCoords[0]) {
                    meshAsset->vertexStream.vertices[iVertex].texcoord = Math::Vector2(mesh->mTextureCoords[0][iVertex].x, mesh->mTextureCoords[0][iVertex].y);
                }
            }

            // ...
            asset->meshes.Add(meshAsset);
        }
    }

    // Diagnostic
    InfoLog(this).Write("Mesh compilation finished (", timer.Delta(), "s)");

    *out = asset;
    return COM::kOK;
}


COM::Result MeshCompiler::Initialize() {
    // Get file system
    if (!registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(this).Write("Expected file system to be installed");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}
