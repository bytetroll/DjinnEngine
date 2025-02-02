//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 5/8/2018.
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