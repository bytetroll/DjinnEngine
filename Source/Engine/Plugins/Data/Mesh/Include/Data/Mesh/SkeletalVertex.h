//
// Created by Dev on 10/23/2017.
//
#pragma once

#include "BaseVertex.h"

namespace Djinn::Data {
    struct SkeletalVertex : public BaseVertex {
        static_assert(sizeof(BaseVertex) == 44, "Unexpected base vertex size");
        Math::Vector4 boneIndices;  // 16  : 60 [4]
        Math::Vector4 boneWeights;  // 16  : 76 [-12]
    };

    /// Serialization helper
    static void operator<<(Serialization::Archive& archive, const SkeletalVertex& value) {
        archive << static_cast<const BaseVertex&>(value);
        archive << value.boneIndices;
        archive << value.boneWeights;
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive& archive, SkeletalVertex& value) {
        archive >> static_cast<BaseVertex&>(value);
        archive >> value.boneIndices;
        archive >> value.boneWeights;
    }
}