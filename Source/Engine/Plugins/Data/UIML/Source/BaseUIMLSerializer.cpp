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
 on 5/5/2018.
//

#include <Data/BaseUIMLSerializer.h>
#include <Data/BaseUIMLAsset.h>
#include <Host/IRegistry.h>
#include <Serialization/Common/Containers.h>
#include <Serialization/Common/VectorMath.h>

DJINN_NS2(Data);

BaseUIMLSerializer::BaseUIMLSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIMLSerializer(registry, outer, host) {

}

COM::Result BaseUIMLSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseUIMLAsset::kCID);

    // ...
    auto asset = static_cast<BaseUIMLAsset *>(object);
    auto &content = asset->content;

    // Serialize main body
    if (!SerializeBody(archive, content.body)) {
        return COM::kError;
    }

    // Serialize styles
    archive << content.styles.Size();
    for (auto &style : content.styles) {
        archive << style.headers.Size();
        for (auto &header : style.headers) {
            archive << header.name;
            archive << static_cast<int>(header.mode);
            archive << static_cast<int>(header.bindMode);
        }

        archive << style.attributes.Size();
        for (auto &attrib : style.attributes) {
            archive << attrib.id;
            if (!SerializeValue(archive, attrib.value)) {
                return COM::kError;
            }
        }

        archive << style.dependencies.Size();
        for (auto &header : style.dependencies) {
            archive << header.name;
            archive << static_cast<int>(header.mode);
            archive << static_cast<int>(header.bindMode);
        }
    }

    // Serialize groups
    archive << content.groups.Size();
    for (auto &group : content.groups) {
        archive << group.id;
        archive << group.name;

        if (!SerializeBody(archive, group.body)) {
            return COM::kError;
        }
    }

    // Serialize components
    archive << content.components.Size();
    for (auto &com : content.components) {
        archive << com.name;
        archive << com.name;
        archive << com.classes;

        archive << static_cast<int>(com.type);
        archive << com.properties.Size();
        for (auto &property : com.properties) {
            archive << property.id;
            archive << property.value;
        }

        if (!SerializeBody(archive, com.body)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result BaseUIMLSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseUIMLAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result BaseUIMLSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseUIMLAsset *>(object);
    auto &content = asset->content;

    // Serialize main body
    if (!DeserializeBody(archive, content.body)) {
        return COM::kError;
    }

    // Serialize styles
    USize styleSize;
    archive >> styleSize;
    content.styles.Resize(styleSize);
    for (USize i = 0; i < styleSize; i++) {
        auto &style = content.styles[i];

        USize headerSize;
        archive >> headerSize;

        style.headers.ResizePassive(headerSize);
        for (USize j = 0; j < headerSize; j++) {
            auto &header = style.headers[j];

            archive >> header.name;

            int mode;
            archive >> mode;
            header.mode = static_cast<Data::UIML::StyleMode >(mode);

            int bindMode;
            archive >> bindMode;
            header.bindMode = static_cast<Data::UIML::StyleBindMode >(bindMode);
        }

        USize attribSize;
        archive >> attribSize;
        style.attributes.Resize(attribSize);
        for (USize j = 0; j < attribSize; j++) {
            auto &attrib = style.attributes[j];

            archive >> attrib.id;

            if (!DeserializeValue(archive, attrib.value)) {
                return COM::kError;
            }
        }

        USize dependenciesSize;
        archive >> dependenciesSize;

        style.dependencies.ResizePassive(dependenciesSize);
        for (USize j = 0; j < dependenciesSize; j++) {
            auto &header = style.dependencies[j];

            archive >> header.name;

            int mode;
            archive >> mode;
            header.mode = static_cast<Data::UIML::StyleMode >(mode);

            int bindMode;
            archive >> bindMode;
            header.bindMode = static_cast<Data::UIML::StyleBindMode >(bindMode);
        }
    }

    // Serialize groups
    USize groupSize;
    archive >> groupSize;
    content.groups.Resize(groupSize);
    for (USize i = 0; i < groupSize; i++) {
        auto &group = content.groups[i];

        archive >> group.id;
        archive >> group.name;

        if (!DeserializeBody(archive, group.body)) {
            return COM::kError;
        }
    }

    // Serialize components
    USize comSize;
    archive >> comSize;
    content.components.Resize(comSize);
    for (USize i = 0; i < comSize; i++) {
        auto &com = content.components[i];

        archive >> com.name;
        archive >> com.classes;

        int type;
        archive >> type;
        com.type = static_cast<Data::UIML::CompiledComponentType>(type);

        USize propertyCount;
        archive >> propertyCount;

        com.properties.ResizePassive(propertyCount);
        for (USize j = 0; j < propertyCount; j++) {
            auto &property = com.properties[j];

            archive >> property.id;
            archive >> property.value;
        }

        if (!DeserializeBody(archive, com.body)) {
            return COM::kError;
        }
    }
    // OK
    return COM::kOK;
}

COM::Result BaseUIMLSerializer::SerializeValue(Serialization::Archive &archive, const Data::UIML::StyleValue &value) {
    archive << value.GetTypeIndex();
    if (value.IsOf<Core::String>()) {
        archive << value.Get<Core::String>().AsCharArray();
    } else if (value.IsOf<float>()) {
        archive << value.Get<float>();
    } else if (value.IsOf<Math::Vector2>()) {
        archive << value.Get<Math::Vector2>();
    } else if (value.IsOf<Math::Vector3>()) {
        archive << value.Get<Math::Vector3>();
    } else if (value.IsOf<Math::Vector4>()) {
        archive << value.Get<Math::Vector4>();
    } else if (value.IsOf<Base::PathID>()) {
        archive << value.Get<Base::PathID>().Get();
    }

    return COM::kOK;
}

COM::Result BaseUIMLSerializer::DeserializeValue(Serialization::Archive &archive, Data::UIML::StyleValue &value) {
    int index;
    archive >> index;

    switch (index) {
        case Data::UIML::StyleValue::Index<Core::String>: {
            char* str;
            archive >> str;
            value = Core::String(str);
            break;
        }
        case Data::UIML::StyleValue::Index<float>: {
            float val;
            archive >> val;
            value = val;
            break;
        }
        case Data::UIML::StyleValue::Index<Math::Vector2>: {
            Math::Vector2 val;
            archive >> val;
            value = val;
            break;
        }
        case Data::UIML::StyleValue::Index<Math::Vector3>: {
            Math::Vector3 val;
            archive >> val;
            value = val;
            break;
        }
        case Data::UIML::StyleValue::Index<Math::Vector4>: {
            Math::Vector4 val;
            archive >> val;
            value = val;
            break;
        }
        case Data::UIML::StyleValue::Index<Base::PathID>: {
            UInt32 val;
            archive >> val;
            value = Base::PathID(val);
            break;
        }
    }

    return COM::kOK;
}

COM::Result BaseUIMLSerializer::SerializeBody(Serialization::Archive &archive, const Data::UIML::CompiledBody &body) {
    archive << body.elements.Size();
    for (auto &element : body.elements) {
        archive << static_cast<int>(element.type);
        archive << element.relativeIndex;
        archive << element.relativeParent.IsValid();
        if (element.relativeParent.IsValid()) {
            archive << element.relativeParent.Get();
        }

        switch (element.type) {
            case UIML::CompiledElementType::eGeneric: {
                archive << element.generic.id;
                archive << element.generic.name;
                archive << element.generic.classes;

                archive << element.generic.properties.Size();
                for (auto &property : element.generic.properties) {
                    archive << property.id;
                    archive << property.value;
                }
                break;
            }
            case UIML::CompiledElementType::eGroup: {
                archive << element.group.name;
                break;
            }
            case UIML::CompiledElementType::eCategory: {
                archive << element.category.name;
                break;
            }
            default:
                break;
        }
    }

    return COM::kOK;
}

COM::Result BaseUIMLSerializer::DeserializeBody(Serialization::Archive &archive, Data::UIML::CompiledBody &body) {
    USize size;
    archive >> size;
    body.elements.Resize(size);

    for (USize i = 0; i < size; i++) {
        auto &element = body.elements[i];

        int type;
        archive >> type;
        element.type = static_cast<Data::UIML::CompiledElementType>(type);

        archive >> element.relativeIndex;

        bool valid;
        archive >> valid;
        if (valid) {
            USize rel;
            archive >> rel;
            element.relativeParent = rel;
        }

        switch (element.type) {
            case UIML::CompiledElementType::eGeneric: {
                archive >> element.generic.id;
                archive >> element.generic.name;
                archive >> element.generic.classes;

                USize propertySize;
                archive >> propertySize;
                element.generic.properties.ResizePassive(propertySize);

                for (USize j = 0; j < propertySize; j++) {
                    archive >> element.generic.properties[j].id;
                    archive >> element.generic.properties[j].value;
                }
                break;
            }
            case UIML::CompiledElementType::eGroup: {
                archive >> element.group.name;
                break;
            }
            case UIML::CompiledElementType::eCategory: {
                archive >> element.category.name;
                break;
            }
            default:
                break;
        }
    }

    return COM::kOK;
}
