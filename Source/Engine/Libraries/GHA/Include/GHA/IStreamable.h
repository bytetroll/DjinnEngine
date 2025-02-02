//
// Created by Dev on 2/8/2018.
//
#pragma once

#include <COM/ID.h>
#include <COM/Result.h>

namespace Djinn::GHA {
    class IStreamable {
    public:
        DJINN_COM_INTERFACEID();

        /// Enumerate the data, does not perform any copy operations
        /// \param size
        /// \param blob
        /// \return
        virtual COM::Result Enumerate(USize& size, void** blob) = 0;

        /// Notify that this streamable has changed
        void Commit() {
            commitIndex++;
        }

        /// Get current commit index of this streamable
        USize GetCommitIndex() const {
            return commitIndex;
        }

    private:
        USize commitIndex = 0;
    };
}