//
// Created by Dev on 1/28/2018.
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