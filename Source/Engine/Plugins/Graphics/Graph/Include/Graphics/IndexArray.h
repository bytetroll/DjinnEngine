//
// Created by Dev on 2/16/2018.
//
#pragma once

#include <Core/Array.h>
#include <GHA/IStreamable.h>
#include <Core/Buffer.h>

namespace Djinn::Graphics {
    template<typename T>
    class IndexArray : public Core::Buffer<T>, public GHA::IStreamable {
    public:
        /// Overrides
        COM::Result Enumerate(USize &size, void **blob) override {
            size = Core::Buffer<T>::ByteSize();
            *blob = Core::Buffer<T>::Ptr();
            return COM::kOK;
        }
    };
}