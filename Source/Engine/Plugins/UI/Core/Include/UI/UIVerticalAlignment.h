//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Annotation.h>
#include <Reflection/Attributes/Proxy.h>

namespace Djinn::UI {
    ClassAttr(
            EnumProxy("eTop", "top"),
            EnumProxy("eBottom", "bottom"),
            EnumProxy("eCenter", "center")
    )
    enum class UIVerticalAlignment : UInt8 {
        eTop, eBottom, eCenter
    };
}