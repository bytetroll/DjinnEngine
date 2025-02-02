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
 on 9/12/2018.
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
