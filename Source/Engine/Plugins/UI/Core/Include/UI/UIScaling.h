//
// Created by Dev on 2/25/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Annotation.h>
#include <Reflection/Attributes/Proxy.h>

namespace Djinn::UI {
    ClassAttr(
            EnumProxy("eNone", "none"),
            EnumProxy("eContent", "content"),
            EnumProxy("eFill", "fill")
    )
    enum class UIScaling : UInt8 {
        // Use default desired size
                eNone,

        // Use local content size, useful for containers, will stretch to fit all sub elements
                eContent,

        // Fill parent container
                eFill
    };
}