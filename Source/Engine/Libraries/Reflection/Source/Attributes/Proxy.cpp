//
// Created by Dev on 9/12/2018.
//

#include <Reflection/Attributes/Proxy.h>
#include <Reflection/IClassInfo.h>
#include <Reflection/IMemberInfo.h>
#include <Reflection/IMethodInfo.h>
#include <Reflection/IEnumInfo.h>
#include <Reflection/IEnumMemberInfo.h>
#include <Core/Common.h>

DJINN_NS2(Attributes);

Proxy::Proxy(const char *name) : name(name) {
    /**/
}

void Proxy::Attached(COM::IQueryable *container) {
    if (auto member = container->QueryInterface<Reflection::IMemberInfo>()) {
        member->GetClass()->AddMemberNameProxy(member, name);
    }

    if (auto method = container->QueryInterface<Reflection::IMethodInfo>()) {
        method->GetClass()->AddMethodNameProxy(method, name);
    }
}

COM::ClassID Proxy::GetClassID() {
    return kCID;
}

EnumProxy::EnumProxy(const char *from, const char *to) : from(from), to(to) {

}

void EnumProxy::Attached(COM::IQueryable *container) {
    if (auto _enum = container->QueryInterface<Reflection::IEnumInfo>()) {
        if (auto member = _enum->GetMember(from)) {
            _enum->AddNameProxy(member, to);
            return;
        }
    }

    DJINN_ASSERT(false, "Invalid enum proxy");
}

COM::ClassID EnumProxy::GetClassID() {
    return Djinn::COM::ClassID();
}
