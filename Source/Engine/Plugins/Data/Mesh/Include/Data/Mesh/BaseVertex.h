//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Math/Vector3.h>
#include <Serialization/Archive.h>
#include <Serialization/Common/VectorMath.h>

namespace Djinn::Data {
    struct BaseVertex {
        BaseVertex& Position(const Math::Vector3& value) {
            position = value;
            return *this;
        }

        BaseVertex& Normal(const Math::Vector3& value) {
            normal = value;
            return *this;
        }

        BaseVertex& Tangent(const Math::Vector3& value) {
            tangent = value;
            return *this;
        }

        BaseVertex& Texcoord(const Math::Vector2& value) {
            texcoord = value;
            return *this;
        }

        Math::Vector3 position;  // 12 : 12 [52]
        Math::Vector3 normal;    // 12 : 24 [40]
        Math::Vector3 tangent;   // 12 : 36 [28]
        Math::Vector2 texcoord;  // 8  : 44 [20]
    };

    /// Serialization helper
    static void operator<<(Serialization::Archive& archive, const BaseVertex& value) {
        archive << value.position;
        archive << value.normal;
        archive << value.tangent;
        archive << value.texcoord;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive& archive, BaseVertex& value) {
        archive >> value.position;
        archive >> value.normal;
        archive >> value.tangent;
        archive >> value.texcoord;
    }
}