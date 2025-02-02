//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Math/Vector2.h>
#include <Graphics/PacketBuffer.h>
#include <Core/Memory.h>
#include <Math/Matrix4.h>

namespace Djinn::Graphics {
    struct SceneDirectionalLightPacket {
        Math::Vector3 direction;
        Math::Vector3 color;
        float intensity = 1.0f;
    };

    /// Buffer helper
    using SceneDirectionalLightPacketBuffer = PacketBuffer<SceneDirectionalLightPacket>;
}