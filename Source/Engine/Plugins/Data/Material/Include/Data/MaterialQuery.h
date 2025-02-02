//
// Created by Dev on 4/6/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Data {
    struct MaterialQuery {
        MaterialQuery& Layout(const COM::ClassID& value) {
            layout = value;
            return *this;
        }

        MaterialQuery& Type(const COM::ClassID& value) {
            type = value;
            return *this;
        }

        UInt64 Hash() const {
            return CombineHash(layout, type);
        }

        COM::ClassID layout;
        COM::ClassID type;
    };
}