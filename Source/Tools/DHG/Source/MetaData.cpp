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
 on 5/31/2018.
//

#include <DHG/MetaData.h>

DJINN_NS2(DHG);

Core::BinaryStream& operator<<(Core::BinaryStream &out, const char *ptr) {
    std::size_t len = std::strlen(ptr);
    out << len;
    out.GetStream()->Write(ptr, len);
    return out;
}

void operator>>(Core::BinaryStream &out, const char *&ptr) {
    std::size_t len;
    out >> len;
    char *_ptr = new char[len + 1];
    _ptr[len] = 0;
    out.GetStream()->Read(_ptr, len);
    ptr = _ptr;
}

void MetaAnnotation::operator>>(Core::BinaryStream &out) {
    out << count;
    out << ignore;
    for (int i = 0; i < count; i++) {
        out << attributes[i].creator;
    }
}

void MetaAnnotation::operator<<(Core::BinaryStream &in) {
    in >> count;
    in >> ignore;
    for (int i = 0; i < count; i++) {
        in >> attributes[i].creator;
    }
}

void MetaType::operator>>(Core::BinaryStream &out) {
    out << layerCount;
    out << namespaceName;
    for (int i = 0; i < layerCount; i++) {
        out << layers[i].isConst;
        out << static_cast<int>(layers[i].type);
        switch (layers[i].type) {
            case MetaType::Layer::eID:
                out << layers[i].id.name;
                out << layers[i].id.namespaceName;
                out << layers[i].id.templateArgumentCount;
                for (UInt32 j = 0; j < layers[i].id.templateArgumentCount; j++) {
                    layers[i].id.templateArguments[j].type >> out;
                    out << layers[i].id.templateArguments[j].isValue;
                    out << layers[i].id.templateArguments[j].value;
                }
                break;
            case MetaType::Layer::eFPtr:
                *layers[i].fptr.returnType >> out;
                out << layers[i].fptr.isConst;
                out << layers[i].fptr.isShorthand;
                out << layers[i].fptr.argumentCount;
                if (layers[i].fptr.classType) {
                    out << true;
                    *layers[i].fptr.classType >> out;
                } else {
                    out << false;
                }
                for (UInt32 j = 0; j < layers[i].fptr.argumentCount; j++) {
                    layers[i].fptr.arguments[j] >> out;
                }
                break;
            case MetaType::Layer::eArray:
                out << layers[i].arrayDimension;
                break;
            default:
                break;
        }
    }
}

void MetaType::operator<<(Core::BinaryStream &in) {
    in >> layerCount;
    in >> namespaceName;
    for (int i = 0; i < layerCount; i++) {
        in >> layers[i].isConst;
        int type;
        in >> type;
        layers[i].type = static_cast<MetaType::Layer::Type>(type);
        switch (layers[i].type) {
            case MetaType::Layer::eID:
                in >> layers[i].id.name;
                in >> layers[i].id.namespaceName;
                in >> layers[i].id.templateArgumentCount;

                if (layers[i].id.templateArgumentCount > 0) {
                    layers[i].id.templateArguments = new TemplateArgument[layers[i].id.templateArgumentCount];
                    for (UInt32 j = 0; j < layers[i].id.templateArgumentCount; j++) {
                        layers[i].id.templateArguments[j].type << in;
                        in >> layers[i].id.templateArguments[j].isValue;
                        in >> layers[i].id.templateArguments[j].value;
                    }
                }
                break;
            case MetaType::Layer::eFPtr:
                *(layers[i].fptr.returnType = new MetaType()) << in;
                in >> layers[i].fptr.isConst;
                in >> layers[i].fptr.isShorthand;
                in >> layers[i].fptr.argumentCount;

                bool hasClass;
                in >> hasClass;
                if (hasClass) {
                    *(layers[i].fptr.classType = new MetaType()) << in;
                } else {
                    layers[i].fptr.classType = nullptr;
                }

                layers[i].fptr.arguments = new MetaType[layers[i].fptr.argumentCount];
                for (UInt32 j = 0; j < layers[i].fptr.argumentCount; j++) {
                    layers[i].fptr.arguments[j] << in;
                }
                break;
            case MetaType::Layer::eArray:
                in >> layers[i].arrayDimension;
                break;
            default:
                break;
        }
    }
}

void MetaMember::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    type >> out;
}

void MetaMember::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    type << in;
}

void MetaEnumMember::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    out << value;
}

void MetaEnumMember::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    in >> value;
}

void MetaParameter::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    type >> out;
}

void MetaParameter::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    type << in;
}

void MetaMethod::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    returnType >> out;
    out << isConst;

    out << parameters.Size();
    for (auto &param : parameters) {
        param >> out;
    }
}

void MetaMethod::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    returnType << in;
    in >> isConst;

    USize size;
    in >> size;

    parameters.Resize(size);
    for (USize i = 0; i < size; i++) {
        parameters[i] << in;
    }
}

void MetaClass::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    out << namespaceName;

    out << members.Size();
    for (auto &param : members) {
        param >> out;
    }

    out << methods.Size();
    for (auto &param : methods) {
        param >> out;
    }

    out << bases.Size();
    for (auto &param : bases) {
        out << param;
    }
}

void MetaClass::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    in >> namespaceName;

    USize size;
    in >> size;

    members.Resize(size);
    for (USize i = 0; i < size; i++) {
        members[i] << in;
    }

    in >> size;

    methods.Resize(size);
    for (USize i = 0; i < size; i++) {
        methods[i] << in;
    }

    in >> size;

    bases.Resize(size);
    for (USize i = 0; i < size; i++) {
        in >> bases[i];
    }
}

void MetaEnum::operator>>(Core::BinaryStream &out) {
    annotation >> out;
    out << name;
    out << namespaceName;
    type >> out;

    out << members.Size();
    for (auto &param : members) {
        param >> out;
    }
}

void MetaEnum::operator<<(Core::BinaryStream &in) {
    annotation << in;
    in >> name;
    in >> namespaceName;
    type << in;

    USize size;
    in >> size;

    members.Resize(size);
    for (USize i = 0; i < size; i++) {
        members[i] << in;
    }
}

void MetaData::operator>>(Core::BinaryStream &out) {
    out << classes.Size();
    for (auto &param : classes) {
        *param >> out;
    }

    out << enums.Size();
    for (auto &param : enums) {
        *param >> out;
    }

    out << namespaceAliases.Size();
    for (auto &param : namespaceAliases) {
        out << param;
    }

    out << includeEdges.Size();
    for (auto &edge : includeEdges) {
        out << edge.first << edge.second;
    }
}

void MetaData::operator<<(Core::BinaryStream &in) {
    USize size;
    in >> size;

    for (USize i = 0; i < size; i++) {
        auto _class = new MetaClass();
        *_class << in;
        classes.Add(_class);
    }

    in >> size;

    for (USize i = 0; i < size; i++) {
        auto _enum = new MetaEnum();
        *_enum << in;
        enums.Add(_enum);
    }

    in >> size;

    for (USize i = 0; i < size; i++) {
        const char* ptr;
        in >> ptr;
        namespaceAliases.Add(ptr);
    }

    in >> size;

    for (USize i = 0; i < size; i++) {
        const char* from;
        in >> from;
        const char* to;
        in >> to;
        includeEdges.Add({from, to});
    }
}
