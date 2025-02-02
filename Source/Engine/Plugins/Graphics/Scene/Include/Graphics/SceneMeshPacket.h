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
#include <Data/Asset.h>

namespace Djinn::Data {
    class IMeshAsset;
    class IMaterialAsset;
}

namespace Djinn::Graphics {
    struct SceneMeshPacket {
        Math::Matrix4 transform = Math::Matrix4::Identity();
        Data::AssetView<Data::IMeshAsset> mesh;
        Data::AssetView<Data::IMaterialAsset> material;
    };

    /// Buffer helper
    using SceneMeshPacketBuffer = PacketBuffer<SceneMeshPacket>;
}