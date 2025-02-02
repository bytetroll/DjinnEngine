//
// Created by Dev on 12/14/2017.
//
#pragma once

#include <Core/Size.h>

namespace Djinn {
    static USize operator "" _kb(USize kb) {
        return kb * 1000;
    }

    static USize operator "" _mb(USize mb) {
        return mb * 1000000;
    }

    static USize operator "" _gb(USize gb) {
        return gb * 1000000000;
    }
}