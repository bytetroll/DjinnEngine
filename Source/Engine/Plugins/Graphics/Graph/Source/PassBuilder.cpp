#include <Graphics/PassBuilder.h>
#include <Core/Common.h>
#include <GHA/ScheduledNode.h>
#include <GHA/ITexture.h>
#include <GHA/IVertexBuffer.h>
#include <GHA/IIndexBuffer.h>
#include <GHA/SchedulerContext.h>
#include <Data/IMeshAsset.h>
#include <Data/ITextureAsset.h>
#include <GHA/IStreamable.h>
#include <GHA/ShaderBlob.h>
#include <GHA/IShader.h>
#include <Data/ShaderAsset.h>

DJINN_NS2(Graphics);

PassBuilder PassBuilder::Make(const char *name, Core::LazyObjectPool<PassNode> *pool, PassContext *passContext, GHA::SchedulerContext *schedulerContext) {
    PassBuilder builder;
    builder.pool = pool;
    builder.passContext = passContext;
    builder.schedulerContext = schedulerContext;
    builder.passNode = pool->Pop();
    builder.passNode->Flush();
    if (name) {
        builder.passNode->node->name = name;
    }
    return builder;
}

PassBuilder::~PassBuilder() {
}

PassBuilder PassBuilder::Pass(const char *name) {
    return PassBuilder::Make(name, pool, passContext, schedulerContext);
}

PassBuilder PassBuilder::InlinedPass(const char *name) {
    PrepareSync();

    auto pass = PassBuilder::Make(name, pool, passContext, schedulerContext);
    pass.passNode->node->dependencies.Add(passNode->node);
    passNode->syncNode->node->dependencies.Add(pass.passNode->node);
    return pass;
}

Texture PassBuilder::AddTexture(GHA::ITexture *texture) {
    DJINN_ASSERT(texture != nullptr, "Invalid texture");
    USize uid;
    if (schedulerContext->CreateImportedUID(texture, uid)) {
        GHA::ResourceCommand command;
        command.importTexture = {};
        command.importTexture.texture = texture;
        command.importTexture.resource = {uid};
        schedulerContext->resources.Add({passNode->node, command});
    }
    return Texture({uid}, texture->GetDesc());
}

Texture PassBuilder::AddTexture(const Data::AssetView<Data::ITextureAsset> &_asset) {
    auto asset = _asset.Get();
    if (!asset) {
        USize uid;
        if (schedulerContext->CreateImportedUID(asset, uid)) {
            GHA::ResourceCommand command;
            command.importTextureStreamable = {};
            command.importTextureStreamable.desc = {};
            command.importTextureStreamable.uid = reinterpret_cast<USize>(asset);
            command.importTextureStreamable.resource = {uid};
            schedulerContext->resources.Add({passNode->node, command});
        }
        return Texture({uid}, {});
    }

    auto desc = GHA::TextureDesc()
            .Format(asset->GetFormat())
            .Size(asset->GetSize())
            .MipLevels(asset->GetMipCount());

    USize uid;
    if (schedulerContext->CreateImportedUID(asset, uid)) {
        GHA::ResourceCommand command;
        command.importTextureStreamable = {};
        command.importTextureStreamable.desc = desc;
        command.importTextureStreamable.uid = reinterpret_cast<USize>(asset);

        // Get streamables
        USize count;
        asset->GetMipStreamables(count, nullptr);

        // Get streamables
        command.importTextureStreamable.streamables.Resize(count);
        asset->GetMipStreamables(count, command.importTextureStreamable.streamables.Ptr());

        // ...
        command.importTextureStreamable.resource = {uid};
        schedulerContext->resources.Add({passNode->node, command});
    }
    return Texture({uid}, desc);
}

VertexBuffer PassBuilder::AddVertexBuffer(GHA::IStreamable *stream, const GHA::VertexBufferDesc &desc, bool highFrequency) {
    USize uid;
    if (schedulerContext->CreateImportedUID(stream, uid)) {
        GHA::ResourceCommand vbuffer;
        vbuffer.importVertexStreamable = {};
        vbuffer.importVertexStreamable.vertexStreamable = stream;
        vbuffer.importVertexStreamable.vertexResource = {uid};
        vbuffer.importVertexStreamable.vertexDesc = desc;
        vbuffer.importVertexStreamable.uid = reinterpret_cast<USize>(stream);
        vbuffer.importVertexStreamable.highFrequency = highFrequency;
        schedulerContext->resources.Add({passNode->node, vbuffer});
    }
    return VertexBuffer({uid}, desc);
}

IndexBuffer PassBuilder::AddIndexBuffer(GHA::IStreamable *stream, const GHA::IndexBufferDesc &desc, bool highFrequency) {
    USize uid;
    if (schedulerContext->CreateImportedUID(stream, uid)) {
        GHA::ResourceCommand ibuffer;
        ibuffer.importIndexStreamable = {};
        ibuffer.importIndexStreamable.indexStreamable = stream;
        ibuffer.importIndexStreamable.indexResource = {uid};
        ibuffer.importIndexStreamable.indexDesc = desc;
        ibuffer.importIndexStreamable.uid = reinterpret_cast<USize>(stream);
        ibuffer.importIndexStreamable.highFrequency = highFrequency;
        schedulerContext->resources.Add({passNode->node, ibuffer});
    }
    return IndexBuffer({uid}, desc);
}

Mesh PassBuilder::AddMesh(const Data::AssetView<Data::IMeshAsset> &_asset) {
    auto asset = _asset.Get();
    if (!asset) {
        USize uid;
        if (schedulerContext->CreateImportedUID(asset, uid)) {
            GHA::ResourceCommand vbuffer;
            vbuffer.importVertexStreamable = {};
            vbuffer.importVertexStreamable.vertexStreamable = nullptr;
            vbuffer.importVertexStreamable.vertexResource = {uid};
            vbuffer.importVertexStreamable.vertexDesc = {};
            vbuffer.importVertexStreamable.uid = reinterpret_cast<USize>(asset);
            schedulerContext->resources.Add({passNode->node, vbuffer});

            GHA::ResourceCommand ibuffer;
            ibuffer.importIndexStreamable = {};
            ibuffer.importIndexStreamable.indexStreamable = nullptr;
            ibuffer.importIndexStreamable.indexResource = {uid + 1};
            ibuffer.importIndexStreamable.indexDesc = {};
            ibuffer.importIndexStreamable.uid = reinterpret_cast<USize>(asset);
            schedulerContext->resources.Add({passNode->node, ibuffer});
        }
        return Mesh(
                {{uid}, {}},
                {{uid + 1}, {}}
        );
    }

    GHA::VertexBufferDesc vertexDesc;
    GHA::IndexBufferDesc indexDesc;

    // Strides
    vertexDesc.stride = asset->GetVertexStride();
    indexDesc.type = asset->GetIndexType();

    // Enumerate
    void *_dummy;
    Sink(asset->GetVertexStream()->Enumerate(vertexDesc.count, &_dummy));
    Sink(asset->GetIndexStream()->Enumerate(indexDesc.count, &_dummy));

    // Calculate counts
    vertexDesc.count /= vertexDesc.stride;
    indexDesc.count /= static_cast<UInt32>(indexDesc.type);

    // Submit commands
    USize uid;
    if (schedulerContext->CreateImportedUID(asset, uid, 2)) {
        GHA::ResourceCommand vbuffer;
        vbuffer.importVertexStreamable = {};
        vbuffer.importVertexStreamable.vertexStreamable = asset->GetVertexStream();
        vbuffer.importVertexStreamable.vertexResource = {uid};
        vbuffer.importVertexStreamable.vertexDesc = vertexDesc;
        vbuffer.importVertexStreamable.uid = reinterpret_cast<USize>(asset);
        schedulerContext->resources.Add({passNode->node, vbuffer});

        GHA::ResourceCommand ibuffer;
        ibuffer.importIndexStreamable = {};
        ibuffer.importIndexStreamable.indexStreamable = asset->GetIndexStream();
        ibuffer.importIndexStreamable.indexResource = {uid + 1};
        ibuffer.importIndexStreamable.indexDesc = indexDesc;
        ibuffer.importIndexStreamable.uid = reinterpret_cast<USize>(asset);
        schedulerContext->resources.Add({passNode->node, ibuffer});
    }
    return Mesh(
            {{uid}, vertexDesc},
            {{uid + 1}, indexDesc}
    );
}

VertexBuffer PassBuilder::AddVertexBuffer(GHA::IVertexBuffer *buffer) {
    USize uid;
    if (schedulerContext->CreateImportedUID(buffer, uid)) {
        GHA::ResourceCommand command;
        command.importVertexBuffer = {};
        command.importVertexBuffer.buffer = buffer;
        command.importVertexBuffer.resource = {uid};
        schedulerContext->resources.Add({passNode->node, command});
    }
    return VertexBuffer({uid}, buffer->GetDesc());
}

IndexBuffer PassBuilder::AddIndexBuffer(GHA::IIndexBuffer *buffer) {
    USize uid;
    if (schedulerContext->CreateImportedUID(buffer, uid)) {
        GHA::ResourceCommand command;
        command.importIndexBuffer = {};
        command.importIndexBuffer.buffer = buffer;
        command.importIndexBuffer.resource = {uid};
        schedulerContext->resources.Add({passNode->node, command});
    }
    return IndexBuffer({uid}, buffer->GetDesc());
}

void PassBuilder::BindShader(GHA::IShader *shader) {
    GHA::Command command;
    command.bindShader = {};
    command.bindShader.shader = shader;
    passNode->node->commands.Add(command);

    // Bind resources
    BindBlobResources(shader->GetDesc().blob);
}

void PassBuilder::BindShader(const Data::AssetView<Data::ShaderAsset> &_asset) {
    auto asset = _asset.Get();
    if (!asset) {
        passNode->isInvalid = true;
        return;
    }

    for (const auto &blob : asset->shaderBlobs) {
        GHA::Command command;
        command.bindShaderBlob = {};
        command.bindShaderBlob.blob = blob.second;
        passNode->node->commands.Add(command);

        // Bind resources
        BindBlobResources(blob.second);
    }
}

PassBuilder &PassBuilder::AddDependency(const PassBuilder &other) {
    if (other.passNode->syncNode) {
        passNode->node->dependencies.Add(other.passNode->syncNode->node);
    } else {
        passNode->node->dependencies.Add(other.passNode->node);
    }
    return *this;
}

void PassBuilder::PrepareSync() {
    if (!passNode->syncNode) {
        passNode->syncNode = pool->Pop();
        passNode->syncNode->Flush();
        passNode->syncNode->node->dependencies.Add(passNode->node);
    }
}

void PassBuilder::MapSharedTexture(GHA::BindID bind, GHA::TextureID id, const GHA::TextureViewDesc &view, const GHA::SamplerDesc &sampler) {
    PassContext::TextureBind passBind;
    passBind.id = id;
    passBind.desc = view;
    passBind.sampler = sampler;
    passContext->mappedTextures.Add(bind, passBind);
}

void PassBuilder::BindBlobResources(GHA::ShaderBlob *blob) {
    for (auto &bind : blob->bindings) {
        auto it = passContext->mappedTextures.Find(bind.id);
        if (it == passContext->mappedTextures.End()) {
            continue;
        }

        // Bind the texture
        BindTexture(bind.target, it->id, it->desc, it->sampler);
    }
}

void PassContext::Clear() {
    mappedTextures.Clear();
}
