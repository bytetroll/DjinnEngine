//
// Created by Dev on 11/12/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/Common.h>
#include <Core/Assert.h>
#include <memory>

namespace Djinn::Memory {
    template<typename T>
    struct Alloca {
        DJINN_NOCOPY(Alloca);

        Alloca(T* ptr) : ptr(ptr) {

        }

        ~Alloca() {
            ptr->~T();
        }

        T* operator->() {
            return ptr;
        }

        operator T*() {
            return ptr;
        }

        T* ptr;
    };

    template<typename T>
    struct AllocaArray {
        DJINN_NOCOPY(AllocaArray);

        AllocaArray(T* ptr, USize size) : ptr(ptr), size(size) {

        }

        ~AllocaArray() {
            for (USize i = 0; i < size; i++) {
                ptr[i].~T();
            }
        }

        T& operator[](USize i) {
            DJINN_ASSERT(i < size, "Stack array index out of bounds");
            return ptr[i];
        }

        operator T*() {
            return ptr;
        }

        T* ptr;
        USize size;
    };

    /// Perform a stack allocation
#define DJINN_ALLOCA(TYPE, ...) Djinn::Memory::Alloca<TYPE>(new (_alloca(TYPE)) TYPE(__VA_ARGS__))

    /// Perform a stack array allocation
#define DJINN_ALLOCA_ARRAY(TYPE, COUNT, ...) Djinn::Memory::AllocaArray<TYPE>(new (_alloca(sizeof(TYPE))) TYPE[COUNT] __VA_ARGS__, COUNT)
}