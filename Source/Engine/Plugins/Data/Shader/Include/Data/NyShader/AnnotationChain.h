//
// Created by Dev on 12/19/2017.
//
#pragma once

#include <Core/Types.h>

namespace NyLang::IL {
    class ConstantMetaValue;
}

namespace Djinn::Data::NyShader {
    struct AnnotationChain {
        void Add(NyLang::IL::ConstantMetaValue* annot) {
            annotations[index++] = annot;
        }

        UInt32 index = 0;
        NyLang::IL::ConstantMetaValue* annotations[16];
    };
}