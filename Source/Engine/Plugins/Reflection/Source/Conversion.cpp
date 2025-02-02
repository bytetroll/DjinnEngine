//
// Created by Dev on 9/12/2018.
//

#include <Reflection/Conversion.h>
#include <Host/IRegistry.h>
#include <Core/Common.h>
#include <Reflection/IReflectionRegistry.h>
#include <Reflection/IEnumInfo.h>

DJINN_NS2(Reflection);

bool Reflection::ToEnum(Host::IRegistry* registry, ClassID cid, USize hash, Int64& out) {
    IReflectionRegistry* refReg;
    if (!registry->GetPipeline()->GetInterface(&refReg)) {
        return false;
    }

    IEnumInfo* info;
    if (!refReg->GetEnum(cid, &info)) {
        return false;
    }

    return info->ToIndex(hash, out);
}
