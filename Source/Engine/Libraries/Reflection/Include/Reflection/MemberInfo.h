//
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

#include "IMemberInfo.h"
#include "IAttribute.h"
#include <algorithm>
#include <Core/TypeName.h>
#include <Core/String.h>

namespace Djinn::Reflection {
    template<typename C, typename T, T C::*ptr>
    class MemberInfo : public IMemberInfo {
    public:
        DJINN_COM_CLASSID();

        MemberInfo(IClassInfo* owner, TypeInfo type, const char* name) : type(type), name(name), owner(owner) {
            decoratedName = Core::String(GetTypeName<T>()) + " " + GetTypeName<C>() + "::" + name;
            hash = Core::ComputeCRC64(name);
        }

        IClassInfo *GetClass() override {
            return owner;
        }

        const TypeInfo& GetType() override {
            return type;
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

        Core::DynamicMember GetDynamic(void *frame) override {
            return Core::Member<C, T>::template Make<ptr>(static_cast<C*>(frame));
        }

        COM::IUnknown *GetCom(void *frame) override {
            if constexpr(std::is_convertible_v<T, COM::IUnknown*>) {
                return static_cast<COM::IUnknown*>(static_cast<C*>(frame)->*ptr);
            }
            return nullptr;
        }

        void *GetRaw(void *frame) override {
            return &(static_cast<C*>(frame)->*ptr);
        }

        USize GetHash() override {
            return hash;
        }

        Core::String ToString(void *frame) override {
            auto& value = static_cast<C*>(frame)->*ptr;
            if constexpr (std::is_arithmetic_v<T>) {
                return Core::String(value);
            } else if constexpr (std::is_same_v<Core::String, std::decay_t<T>> || std::is_same_v<char*, std::decay_t<T>>) {
                return value;
            } else {
                return decoratedName;
            }
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
        TypeInfo type;
        Core::String name;
        Core::String decoratedName;
        IClassInfo* owner;
    };
}