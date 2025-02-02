//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Data/IAsset.h>
#include <GHA/IndexBufferDesc.h>

namespace Djinn::GHA {
    class IStreamable;
}

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH IMeshAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

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
                case IMeshAsset::kIID:
                    *out = static_cast<IMeshAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

        /// Get layout com class id
        /// \return
        virtual COM::ClassID GetLayoutClassID() = 0;

        /// Get vertex stride
        /// \return
        virtual USize GetVertexStride() = 0;

        /// Get index type
        /// \return
        virtual GHA::IndexType GetIndexType() = 0;

        /// Get vertex streamable
        /// \return
        virtual GHA::IStreamable *GetVertexStream() = 0;

        /// Get index streamable
        /// \return
        virtual GHA::IStreamable *GetIndexStream() = 0;
    };
}