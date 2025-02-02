//
// Created by Dev on 10/19/2017.
//

#include <Reflection/ReflectionRegistry.h>
#include <Reflection/IClassInfo.h>
#include <Reflection/IEnumInfo.h>
#include <Reflection/IAttribute.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS2(Reflection);

ReflectionRegistry::ReflectionRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IReflectionRegistry(registry, outer, host) {

}

COM::Result ReflectionRegistry::AddClass(IClassInfo *_class) {
    classes[_class->GetReflectionID()] = _class;

    // Populate attributes
    for (auto attr : _class->GetAttributes()) {
        attributeClasses[ClassID(attr->GetClassID())].Add(_class);
    }

    // Populate bases
    for (auto _base : _class->GetBases()) {
        IClassInfo* base;
        if (GetClass(_base, &base)) {
            derivedClasses[_class->GetClassID()].Add(base);
        }
    }

    // Register callbacks
    for (auto attr : _class->GetAttributes()) {
        if (auto visitor = classAttrBound.GetNullable(attr->GetClassID())) {
            visitor->Visit(_class, attr);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result ReflectionRegistry::GetClass(const ClassID &id, IClassInfo **out) {
    auto it = classes.Find(id);
    if (it == classes.End()) {
        return COM::kError;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result ReflectionRegistry::RemoveClass(const ClassID &id) {
    classes.Remove(id);
    return COM::kOK;
}

COM::Result ReflectionRegistry::VisitDerivedClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *)> &visitor) {
    auto it = derivedClasses.Find(id);
    if (it == derivedClasses.End()) {
        return COM::kError;
    }

    for (auto info : (*it).second) {
        visitor(info);
    }
    return COM::kOK;
}

COM::Result ReflectionRegistry::VisitAttributeClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute*)> &visitor) {
    auto it = attributeClasses.Find(id);
    if (it == attributeClasses.End()) {
        return COM::kError;
    }

    for (auto info : (*it).second) {
        if (auto attr = info->GetAttribute(id)) {
            visitor(info, attr);
        }
    }
    return COM::kOK;
}

COM::Result ReflectionRegistry::AddEnum(IEnumInfo *_enum) {
    enums[_enum->GetReflectionID()] = _enum;

    // Populate attributes
    for (auto attr : _enum->GetAttributes()) {
        attributeEnums[ClassID(attr->GetClassID())].Add(_enum);
    }

    // Register callbacks
    for (auto attr : _enum->GetAttributes()) {
        if (auto visitor = enumAttrBound.GetNullable(attr->GetClassID())) {
            visitor->Visit(_enum, attr);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result ReflectionRegistry::GetEnum(const ClassID &id, IEnumInfo **out) {
    auto it = enums.Find(id);
    if (it == enums.End()) {
        return COM::kError;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result ReflectionRegistry::VisitAttributeEnums(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor) {
    auto it = attributeEnums.Find(id);
    if (it == attributeEnums.End()) {
        return COM::kError;
    }

    for (auto info : (*it).second) {
        if (auto attr = info->GetAttribute(id)) {
            visitor(info, attr);
        }
    }
    return COM::kOK;
}

COM::Result ReflectionRegistry::RemoveEnum(const ClassID &id) {
    enums.Remove(id);
    return COM::kOK;
}

COM::Result ReflectionRegistry::BindClassAttributeInstalled(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute *)> &visitor, bool invoke) {
    classAttrBound.Add(id, visitor);
    if (invoke) {
        return VisitAttributeClasses(id, visitor);
    }
    return COM::kOK;
}

COM::Result ReflectionRegistry::BindEnumAttributeInstalled(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor, bool invoke) {
    enumAttrBound.Add(id, visitor);
    if (invoke) {
        return VisitAttributeEnums(id, visitor);
    }
    return COM::kOK;
}
