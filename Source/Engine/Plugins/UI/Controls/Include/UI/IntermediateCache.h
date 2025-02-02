//
// Created by Dev on 11/12/2018.
//
#pragma once

#include <Data/GlyphConfig.h>
#include <Core/Array.h>

namespace Djinn::UI {
    struct IntermediateCache {
        Core::Array<Data::GlyphRender> glyphRenders;
    };
}