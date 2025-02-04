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
 on 5/28/2018.
//
#pragma once

#include <Core/Array.h>
#include <Core/BinaryStream.h>
#include <Core/Pair.h>
#include <Core/HashSet.h>

#include <clang-c/Index.h>

namespace Djinn {
    template<>
    struct Hasher<CXCursor> {
        USize operator()(const CXCursor& value) const {
            return clang_hashCursor(value);
        }
    };

    template<>
    struct Comparator<CXCursor> {
        USize operator()(const CXCursor& lhs, const CXCursor& rhs) const {
            return clang_equalCursors(lhs, rhs);
        }
    };
}

namespace Djinn::DHG {
    struct MetaAnnotation {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        int count = 0;

        bool ignore = false;

        struct Attribute {
            const char* creator = nullptr;
        } attributes[12];
    };

    struct MetaType {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        int layerCount = 0;

        // First layer is top layer
        struct Layer {
            enum Type {
                eID,
                eVoid,
                eBool,
                eUChar,
                eChar,
                eShort,
                eUShort,
                eLong,
                eLongLong,
                eULong,
                eULongLong,
                eWChar,
                eFloat,
                eDouble,
                eInt,
                eUInt,
                ePointer,
                eRef,
                eArray,
                eFPtr,
            } type;

            bool isConst = false;

            union {
                struct {
                    const char* name;
                    const char* namespaceName;
                    struct TemplateArgument* templateArguments;
                    UInt32 templateArgumentCount;
                } id;
                struct {
                    MetaType* returnType;
                    MetaType* classType;
                    MetaType* arguments;
                    UInt32 argumentCount;
                    bool isConst;
                    bool isShorthand;
                } fptr;
                long long arrayDimension;
            };
        } layers[12];

        const char* namespaceName = nullptr;
    };

    struct TemplateArgument {
        bool isValue = false;
        MetaType type = {};
        long long value = 0;
    };

    struct MetaMember {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        const char* name = nullptr;
        MetaType type;
    };

    struct MetaParameter {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        MetaType type;
        const char* name = nullptr;
    };

    struct MetaMethod {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        const char* name = nullptr;
        bool isConst = false;
        MetaType returnType;
        Core::Array<MetaParameter> parameters;
    };

    struct MetaClass {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        const char* name = nullptr;
        const char* namespaceName = nullptr;
        Core::Array<MetaMember> members;
        Core::Array<MetaMethod> methods;
        Core::Array<const char*> bases;
    };

    struct MetaEnumMember {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        const char* name = nullptr;
        Int64 value = 0;
    };

    struct MetaEnum {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        MetaAnnotation annotation;
        const char* name = nullptr;
        const char* namespaceName = nullptr;
        MetaType type;
        Core::Array<MetaEnumMember> members;
    };

    struct MetaData {
        void operator>>(Core::BinaryStream& out);
        void operator<<(Core::BinaryStream& in);

        Core::Array<const char*> namespaceAliases;
        Core::Array<MetaClass*> classes;
        Core::Array<MetaEnum*> enums;
        Core::Array<Core::Pair<const char*, const char*>> includeEdges;

        Core::HashSet<CXCursor> reflectedMap;
    };
}