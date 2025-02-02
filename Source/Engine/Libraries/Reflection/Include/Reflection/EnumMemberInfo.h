//
// Created by Dev on 5/29/2018.
//
#pragma once

#include "IEnumMemberInfo.h"
#include "IAttribute.h"
#include <algorithm>
#include <Core/TypeName.h>
#include <Core/String.h>

namespace Djinn::Reflection {
    template<typename T>
    class EnumMemberInfo : public IEnumMemberInfo {
    public:
        DJINN_COM_CLASSID();

        EnumMemberInfo(IEnumInfo* owner, const char* name, Int64 value) : owner(owner), name(name), value(value) {
            decoratedName = Core::String(GetTypeName<T>()) + "::" + name;
            hash = Core::ComputeCRC64(name);
        }

        IEnumInfo *GetEnum() override {
            return owner;
        }

        Int64 GetValue() override {
            return value;
        }

        const char *GetName() override {
            return name.AsCharArray();
        }

        const char *GetDecoratedName() override {
            return decoratedName.AsCharArray();
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

        USize GetHash() override {
            return hash;
        }

        void AddAttribute(IAttribute *attribute) {
            attributes.Add(attribute);
            attribute->Attached(this);
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        USize hash;
        Core::Array<IAttribute*> attributes;
        Int64 value;
        Core::String name;
        Core::String decoratedName;
        IEnumInfo* owner;
    };
}