//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/Size.h>

namespace Djinn::GHA {
    struct VertexBufferDesc {
        VertexBufferDesc(USize stride = 0, USize count = 0) : stride(stride), count(count) {

        }

        template<typename T>
        static VertexBufferDesc Make(UInt32 count) {
            return VertexBufferDesc().Stride(sizeof(T)).Count(count);
        }

        VertexBufferDesc& Stride(USize value) {
            stride = value;
            return *this;
        }

        VertexBufferDesc& Count(USize value) {
            count = value;
            return *this;
        }

        VertexBufferDesc& HostVisible(bool value = true) {
            hostVisible = value;
            return *this;
        }

        // Element stride
        USize stride = 16;

        // Number of elements
        USize count = 0;

        // Host visible?
        bool hostVisible = false;
    };
}