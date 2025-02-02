//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Serialization/Archive.h>

namespace Djinn::Math {
    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector2 &vec) {
        archive << vec.x << vec.y;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector2 &out) {
        archive >> out.x >> out.y;
    }

    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector3 &vec) {
        archive << vec.x << vec.y << vec.z;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector3 &out) {
        archive >> out.x >> out.y >> out.z;
    }

    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Vector4 &vec) {
        archive << vec.x << vec.y << vec.z << vec.w;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Vector4 &out) {
        archive >> out.x >> out.y >> out.z >> out.w;
    }
}