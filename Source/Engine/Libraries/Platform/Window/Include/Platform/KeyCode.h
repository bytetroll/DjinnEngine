//
// Created by Dev on 1/12/2018.
//
#pragma once

#include <Core/Types.h>
#include "VirtualKeyCode.h"

namespace Djinn::Platform {
    enum class KeyCode {
        eNone,
        eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ,
    };

    struct Key {
        Key() {

        }

        Key(VirtualKeyCode virtualKeyCode) : virtualKeyCode(virtualKeyCode) {

        }

        Key(KeyCode keyCode) : keyCode(keyCode) {

        }

        VirtualKeyCode virtualKeyCode = Platform::VirtualKeyCode::eCharacter;
        KeyCode keyCode = KeyCode::eNone;
    };
}

namespace Djinn {
    /// Common hashes
    template<> struct Hasher<Platform::KeyCode> { USize operator()(const Platform::KeyCode& value) const { return Hasher<UInt64>{}(static_cast<UInt64>(value)); }};
}