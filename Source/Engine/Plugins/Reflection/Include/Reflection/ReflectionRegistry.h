//
// Created by Dev on 10/19/2017.
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