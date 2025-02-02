//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Annotation.h>
#include <Reflection/Attributes/Proxy.h>

namespace Djinn::UI {
    ClassAttr(
            EnumProxy("eLeft", "left"),
            EnumProxy("eRight", "right"),
            EnumProxy("eCenter", "center")
    )
    enum class UIHorizontalAlignment : UInt8 {
        eLeft,
        eRight,
        eCenter
    };
}