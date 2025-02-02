//
// Created by Dev on 5/9/2018.
//
#pragma once

#include <Math/Vector4.h>
#include <Base/Paths.h>
#include <Core/Variant.h>
#include <Core/StringView.h>

namespace Djinn::Data::UIML {
    /// Style property
    using StyleValue = Core::Variant<Core::String, float, Math::Vector2, Math::Vector3, Math::Vector4, Base::PathID>;

    struct StyleAttribute {
        USize id;
        StyleValue value;
    };
}
