//
// Created by Dev on 1/12/2018.
//
#pragma once

#include <Core/Hash.h>

namespace Djinn::Platform {
    enum class VirtualKeyCode {
        eCharacter,
        eMouseLeft,
        eMouseRight,
        eMouseScroll,
        eBackspace,
        eClear,
        eEnter,
        eShift,
        eCtrl,
        eAlt,
        ePause,
        eCapsLock,
        eEscape,
        eSpacebar,
        ePageUp,
        ePageDown,
        eHome,
        eArrowLeft,
        eArrowRight,
        eArrowUp,
        eArrowDown,
        eSelect,
        ePrint,
        ePrintScreen,
        eInsert,
        eDelete,
    };
}

namespace Djinn {
    /// Common hashes
    template<> struct Hasher<Platform::VirtualKeyCode> { USize operator()(const Platform::VirtualKeyCode& value) const { return Hasher<UInt64>{}(static_cast<UInt64>(value)); }};
}