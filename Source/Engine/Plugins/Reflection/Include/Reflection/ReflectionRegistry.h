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
 on 10/19/2017.
//
#pragma once

#include <Core/HashMap.h>
#include <Core/Array.h>
#include <Reflection/IReflector.h>
#include "IReflectionRegistry.h"

namespace Djinn::Reflection {
    class ReflectionRegistry : public IReflectionRegistry {
    public:
        DJINN_COM_CLASS();

        ReflectionRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result AddClass(IClassInfo *_class) override;
        COM::Result GetClass(const ClassID &id, IClassInfo **out) override;
        COM::Result RemoveClass(const ClassID &id) override;
        COM::Result VisitDerivedClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *)> &visitor) override;
        COM::Result VisitAttributeClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute*)> &visitor) override;
        COM::Result AddEnum(IEnumInfo *_enum) override;
        COM::Result GetEnum(const ClassID &id, IEnumInfo **out) override;
        COM::Result VisitAttributeEnums(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor) override;
        COM::Result RemoveEnum(const ClassID &id) override;
        COM::Result BindClassAttributeInstalled(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute *)> &visitor, bool invoke) override;
        COM::Result BindEnumAttributeInstalled(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor, bool invoke) override;

    private:
        Core::HashMap<ClassID, IClassInfo*> classes;
        Core::HashMap<ClassID, IEnumInfo*> enums;
        Core::HashMap<ClassID, Core::Array<IClassInfo*>> derivedClasses;
        Core::HashMap<ClassID, Core::Array<IClassInfo*>> attributeClasses;
        Core::HashMap<ClassID, Core::Array<IEnumInfo*>> attributeEnums;
        Core::HashMap<ClassID, Core::Visitor<void(IClassInfo *, IAttribute *)>> classAttrBound;
        Core::HashMap<ClassID, Core::Visitor<void(IEnumInfo *, IAttribute *)>> enumAttrBound;
    };
}