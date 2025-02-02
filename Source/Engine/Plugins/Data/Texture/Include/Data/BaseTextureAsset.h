//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 1/28/2018.
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