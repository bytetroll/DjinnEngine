//
// Created by Dev on 5/8/2018.
//
#pragma once

#include <Core/String.h>
#include "StyleBindMode.h"
#include "StyleMode.h"

namespace Djinn::Data::UIML {
    struct StyleBind {
        StyleBindMode bindMode;
        StyleMode mode;
        Core::String name;
    };
}