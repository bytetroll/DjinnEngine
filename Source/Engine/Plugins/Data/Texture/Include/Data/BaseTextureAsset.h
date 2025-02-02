//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <GHA/Format.h>
#include <GHA/Size.h>
#include <Core/Array.h>
#include "ITextureAsset.h"
#include <GHA/IStreamable.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_TEXTURE BaseTextureAsset : public ITextureAsset {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        BaseTextureAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
                virtual GHA::Format GetFormat() override;
        virtual GHA::Size GetSize() override;
        virtual UInt32 GetMipCount() override;
        virtual void GetMipStreamables(USize& outCount, GHA::IStreamable** outStreamables) override;

        /// Size of this texture
        GHA::Size size;

        /// Format of the blob
        GHA::Format format = GHA::Format::eUndefined;

        /// Mip map stream
        class DJINN_EXPORT_PLUGINS_DATA_TEXTURE MipMapStream : public GHA::IStreamable {
        public:
            virtual COM::Result Enumerate(USize &size, void **blob) override;

            /// Data
            Core::Array<char> data;
        };

        /// Mip-map blobs
        /// Lowest mip is the largest
        Core::Array<MipMapStream> mipMapBlobs;
    };
}