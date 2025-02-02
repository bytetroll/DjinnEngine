//
// Created by Dev on 5/8/2018.
//
#pragma once

#include <Core/FixedArray.h>
#include <Core/Array.h>
#include <Core/String.h>
#include <Data/UIML/StyleBindMode.h>
#include <Data/UIML/StyleMode.h>
#include <Data/UIML/StyleAttribute.h>
#include <Core/Optional.h>

namespace Djinn::Data::UIML {
    struct CompiledHeader {
        StyleBindMode bindMode;
        StyleMode mode;
        Core::String name;
    };

    struct CompiledStyle {
        Core::FixedArray32<CompiledHeader> headers;
        Core::FixedArray64<StyleAttribute> attributes;
        Core::FixedArray32<CompiledHeader> dependencies;
    };

    struct CompiledProperty {
        Core::String id;
        Core::String value;
    };

    enum class CompiledElementType {
        eGeneric,
        eGroup,
        eCategory,
        eData
    };

    struct CompiledGenericElement {
        Core::String id;
        Core::String name;
        Core::String classes;
        Core::FixedArray32<CompiledProperty> properties;
    };

    struct CompiledDataElement {
        //Value content;
    };

    struct CompiledGroupElement {
        Core::String name;
    };

    struct CompiledCategoryElement {
        Core::String name;
    };

    struct CompiledElement {
        CompiledElementType type;
        CompiledGenericElement generic;
        CompiledDataElement data;
        CompiledGroupElement group;
        CompiledCategoryElement category;
        USize relativeIndex = 0;
        Core::Optional<USize> relativeParent;
    };

    struct CompiledBody {
        Core::Array<CompiledElement> elements;
    };

    struct CompiledComponentProperty {
        Core::String id;
        Core::String value;
    };

    enum class CompiledComponentType {
        eGeneric,
        eData
    };

    struct CompiledComponent {
        Core::String name;
        CompiledComponentType type = CompiledComponentType::eGeneric;
        Core::String classes;
        Core::FixedArray32<CompiledComponentProperty> properties;
        CompiledBody body;
    };

    struct CompiledGroup {
        USize id = 0;
        Core::String name;
        CompiledBody body;
    };
}

namespace Djinn::Data {
    struct UIMLContent {
        Core::Array<UIML::CompiledStyle> styles;
        Core::Array<UIML::CompiledComponent> components;
        Core::Array<UIML::CompiledGroup> groups;
        UIML::CompiledBody body;
    };
}