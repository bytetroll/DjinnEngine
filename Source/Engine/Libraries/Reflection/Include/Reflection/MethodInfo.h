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

#include "IMethodInfo.h"
#include "IAttribute.h"
#include <algorithm>
#include <Core/TypeName.h>
#include <Core/String.h>

namespace Djinn::Reflection {
    template<typename C, typename T, typename... A>
    class MethodCreator {
    public:
        template<T(C::* ptr)(A...)>
        class MethodInfo : public IMethodInfo {
        public:
            DJINN_COM_CLASSID();

            MethodInfo(IClassInfo* owner, TypeInfo returnType, const Core::Array<ParameterInfo> &parameters, const char *name) : owner(owner), type(returnType), parameters(parameters), name(name) {
                decoratedName = Core::String(GetTypeName<C>()) + "::" + name;
                decoratedSignature = Core::String(returnType.GetDecoratedName()) + " " + decoratedName + "(";
                for (int i = 0; i < parameters.Size(); i++) {
                    if (i != 0) {
                        decoratedSignature += ", ";
                    }
                    decoratedSignature += Core::String(parameters[i].GetDecoratedName());
                }
                decoratedSignature += ")";
            }

            IClassInfo *GetClass() override {
                return owner;
            }

            TypeInfo GetReturnType() override {
                return type;
            }

            const char *GetName() override {
                return name.AsCharArray();
            }

            const char *GetDecoratedName() override {
                return decoratedName.AsCharArray();
            }

            const char *GetDecoratedSignature() override {
                return decoratedSignature.AsCharArray();
            }

            Core::ArrayView<ParameterInfo> GetParameters() override {
                return parameters;
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

            Core::DynamicDelegate GetDynamic(void *frame) override {
                return Core::Delegate<T(A...)>::template Make<C, ptr>(static_cast<C*>(frame));
            }

            USize GetSignatureHash() override {
                return GetTypeNameCRC64<T(A...)>();
            }

            void AddAttribute(IAttribute *attribute) {
                attributes.Add(attribute);
                attribute->Attached(this);
            }

            COM::ClassID GetClassID() override {
                return kCID;
            }

            TypeInfo type;
            Core::String name;
            Core::String decoratedName;
            Core::String decoratedSignature;
            Core::Array<ParameterInfo> parameters;
            IClassInfo* owner;

        private:
            Core::Array<IAttribute*> attributes;
        };

        template<T(C::* ptr)(A...) const>
        class ConstMethodInfo : public IMethodInfo {
        public:
            DJINN_COM_CLASSID();

            ConstMethodInfo(IClassInfo* owner, TypeInfo returnType, const Core::Array<ParameterInfo> &parameters, const char *name) : owner(owner), type(returnType), parameters(parameters), name(name) {
                decoratedName = Core::String(GetTypeName<C>()) + "::" + name;
                decoratedSignature = Core::String(returnType.GetDecoratedName()) + " " + decoratedName + "(";
                for (int i = 0; i < parameters.Size(); i++) {
                    if (i != 0) {
                        decoratedSignature += ", ";
                    }
                    decoratedSignature += Core::String(parameters[i].GetDecoratedName());
                }
                decoratedSignature += ")";
            }

            IClassInfo *GetClass() override {
                return owner;
            }

            TypeInfo GetReturnType() override {
                return type;
            }

            const char *GetName() override {
                return name.AsCharArray();
            }

            const char *GetDecoratedName() override {
                return decoratedName.AsCharArray();
            }

            const char *GetDecoratedSignature() override {
                return decoratedSignature.AsCharArray();
            }

            Core::ArrayView<ParameterInfo> GetParameters() override {
                return parameters;
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

            Core::DynamicDelegate GetDynamic(void *frame) override {
                return Core::Delegate<T(A...)>::template Make<C, ptr>(static_cast<C*>(frame));
            }

            USize GetSignatureHash() override {
                return GetTypeNameCRC64<T(A...) const>();
            }

            void AddAttribute(IAttribute *attribute) {
                attributes.Add(attribute);
                attribute->Attached(this);
            }

            COM::ClassID GetClassID() override {
                return kCID;
            }

            TypeInfo type;
            Core::String name;
            Core::String decoratedName;
            Core::String decoratedSignature;
            Core::Array<ParameterInfo> parameters;
            IClassInfo* owner;

        private:
            Core::Array<IAttribute*> attributes;
        };
    };
}