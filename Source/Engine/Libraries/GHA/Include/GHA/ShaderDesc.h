//
// Created by Dev on 12/9/2017.
//
#pragma once

namespace Djinn::GHA {
    struct ShaderBlob;

    struct ShaderDesc {
        /// Set blob
        /// \param blob
        /// \return
        ShaderDesc& Blob(ShaderBlob* blob) {
            this->blob = blob;
            return *this;
        }

        // Blob
        ShaderBlob* blob;
    };
}