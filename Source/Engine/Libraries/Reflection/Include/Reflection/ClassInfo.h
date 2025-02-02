//
// Created by Dev on 5/29/2018.
//
#pragma once

#include "IClassInfo.h"
#include "IAttribute.h"
#include <algorithm>
#include <Core/TypeName.h>
#include <Core/String.h>
#include <Core/Array.h>
#include <Core/StackArray.h>
#include <Core/HashMap.h>
#include <Core/Map.h>
#include "IMemberInfo.h"
#include "IMethodInfo.h"

namespace Djinn::Reflection {
    template<typename T>
    class ClassInfo : public IClassInfo {
    public:
        DJINN_COM_CLASSID();

        ClassInfo(const Core::StackArray8<ClassID> &bases) : bases(bases) {
            classID = ClassID(GetTypeNameCRC64<T>());
            qualifiedName = GetTypeName<T>();
            decoratedName = "class " + qualifiedName;
            name = Core::String(qualifiedName, static_cast<UInt32>(qualifiedName.LastIndexOf(':') + 1), qualifiedName.Length());
        }

        bool IsPrimitive() override {
            return std::is_arithmetic_v<T>;
        }

        USize GetByteSize() override {
            return sizeof(T);
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

        Core::ArrayView<IMemberInfo *> GetMembers() override {
            return members;
        }

        Core::ArrayView<IMethodInfo *> GetMethods() override {
            return methods;
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

        Core::ArrayView<ClassID> GetBases() override {
            return bases;
        }

        IMemberInfo *GetMember(USize hash) override {
            auto it = memberHashMap.Find(hash);
            if (it == memberHashMap.End()) {
                return nullptr;
            }
            return (*it).second;
        }

        IMethodInfo *GetMethod(USize hash, USize signatureHash) override {
            auto it = methodHashMap.Find(CombineHash(hash, signatureHash));
            if (it == methodHashMap.End()) {
                return nullptr;
            }
            return (*it).second;
        }

        void AddMemberNameProxy(IMemberInfo *member, USize proxy) override {
            memberHashMap.Add(proxy, member);
        }

        void AddMethodNameProxy(IMethodInfo *member, USize proxy) override {
            methodHashMap.Add(proxy, member);
        }

        void AddMember(IMemberInfo *member) {
            members.Add(member);
            memberHashMap.Add(Core::ComputeCRC64(member->GetName()), member);
        }

        void AddMethod(IMethodInfo *method) {
            methods.Add(method);
            methodHashMap.Add(CombineHash(Core::ComputeCRC64(method->GetName()), 0), method);
            methodHashMap.Add(CombineHash(Core::ComputeCRC64(method->GetName()), method->GetSignatureHash()), method);
        }

        void AddAttribute(IAttribute *attribute) {
            attributes.Add(attribute);
            attribute->Attached(this);
        }

    private:
        ClassID classID;
        Core::StackArray8<ClassID> bases;
        Core::Array<IAttribute *> attributes;
        Core::String name;
        Core::String qualifiedName;
        Core::String decoratedName;
        Core::Array<IMemberInfo *> members;
        Core::HashMap<USize, IMemberInfo *> memberHashMap;
        Core::Array<IMethodInfo *> methods;
        Core::HashMap<USize, IMethodInfo *> methodHashMap;
    };
}