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

#include <Data/IAsset.h>
#include <GHA/Format.h>
#include <GHA/Size.h>

namespace Djinn::GHA {
    class IStreamable;
}

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_TEXTURE ITextureAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        ITextureAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case ITextureAsset::kIID:
                    *out = static_cast<ITextureAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

        /// Get format of this texture
        /// \return
        virtual GHA::Format GetFormat() = 0;

        /// Get mip count of this texture
        /// \return
        virtual UInt32 GetMipCount() = 0;

        /// Get size of this texture
        /// \return
        virtual GHA::Size GetSize() = 0;

        /// Get mip map streamables
        /// \param outCount
        /// \param outStreamables
        /// \return
        virtual void GetMipStreamables(USize &outCount, GHA::IStreamable **outStreamables) = 0;
    };
}