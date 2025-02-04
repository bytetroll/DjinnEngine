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
 on 5/29/2018.
//
#pragma once

#include "IEnumInfo.h"
#include "IEnumMemberInfo.h"
#include "IAttribute.h"
#include <algorithm>
#include <Core/TypeName.h>
#include <Core/String.h>
#include <Core/Array.h>
#include <Core/StackArray.h>
#include <Core/HashMap.h>
#include <Core/Map.h>

namespace Djinn::Reflection {
    template<typename T>
    class EnumInfo : public IEnumInfo {
    public:
        DJINN_COM_CLASSID();

        EnumInfo(const TypeInfo &typeInfo) : typeInfo(typeInfo) {
            classID = ClassID(GetTypeNameCRC64<T>());
            qualifiedName = GetTypeName<T>();
            decoratedName = "enum " + qualifiedName;
            name = Core::String(qualifiedName, static_cast<UInt32>(qualifiedName.LastIndexOf(':') + 1), qualifiedName.Length());
        }

        USize GetByteSize() override {
            return sizeof(T);
        }

        const TypeInfo &GetUnderlyingType() override {
            return typeInfo;
        }

        const char *GetName() override {
            return name.AsCharArray();
        }

        const char *GetQualifiedName() override {
            return qualifiedName.AsCharArray();
        }

        const char *GetDecoratedName() override {
            return decoratedName.AsCharArray();
        }

        Core::ArrayView<IEnumMemberInfo *> GetMembers() override {
            return members;
        }

        IEnumMemberInfo *GetMember(USize hash) override {
            return memberMap.GetOrDefault(hash, nullptr);
        }

        bool ToString(Int64 value, Core::StringView &out) override {
            if (auto ptr = valueNameMap.GetNullable(value)) {
                out = *ptr;
                return true;
            }
            return false;
        }

        Core::ArrayView<IAttribute *> GetAttributes() override {
            return attributes;
        }

        IAttribute *GetAttribute(const ClassID &id) override {
            for (auto attr : attributes) {
                if (attr->GetClassID().Get() == id.Get()) {
                    return attr;
                }
            }
            return nullptr;
        }

        ClassID GetReflectionID() override {
            return classID;
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        void AddAttribute(IAttribute *attribute) {
            attributes.Add(attribute);
            attribute->Attached(this);
        }

        bool ToIndex(USize hash, Int64 &out) override {
            if (auto ptr = valueMap.GetNullable(hash)) {
                out = *ptr;
                return true;
            }
            return false;
        }

        void AddNameProxy(IEnumMemberInfo *member, USize proxy) override {
            valueMap.Add(proxy, member->GetValue());
            memberMap.Add(proxy, member);
        }

        void AddMember(IEnumMemberInfo *info) {
            members.Add(info);
            valueMap.Add(info->GetHash(), info->GetValue());
            valueNameMap.Add(info->GetValue(), info->GetName());
            memberMap.Add(info->GetHash(), info);
        }

    private:
        ClassID classID;
        Core::Array<IAttribute *> attributes;
        Core::String name;
        Core::String qualifiedName;
        Core::String decoratedName;
        Core::Array<IEnumMemberInfo *> members;
        Core::HashMap<USize, Int64> valueMap;
        Core::HashMap<Int64, Core::String> valueNameMap;
        Core::HashMap<USize, IEnumMemberInfo *> memberMap;
        TypeInfo typeInfo;
    };
}