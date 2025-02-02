//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/Size.h>

namespace Djinn::GHA {
    enum class IndexType {
        eU16 = 2,
        eU32 = 4
    };

    struct IndexBufferDesc {
        IndexBufferDesc(IndexType type = IndexType::eU32, USize count = 0) : type(type), count(count) {

        }

        IndexBufferDesc(USize count) : type(IndexType::eU32), count(count) {

        }

        template<typename T>
        static IndexBufferDesc Make(UInt32 count) {
            if constexpr(Core::IsSame<T, UInt32>::kValue) {
                return IndexBufferDesc().Type(IndexType::eU32).Count(count);
            } else if constexpr (Core::IsSame<T, UInt16>::kValue) {
                return IndexBufferDesc().Type(IndexType::eU16).Count(count);
            } else {
                static_assert("Unsupported index type");
            }
        }

        IndexBufferDesc& Type(IndexType value) {
            type = value;
            return *this;
        }

        IndexBufferDesc& Count(USize value) {
            count = value;
            return *this;
        }

        IndexBufferDesc& HostVisible(bool value = true) {
            hostVisible = value;
            return *this;
        }

        // Element type
        IndexType type;

        // Number of elements
        USize count;

        // Host visible?
        bool hostVisible = false;
    };
}