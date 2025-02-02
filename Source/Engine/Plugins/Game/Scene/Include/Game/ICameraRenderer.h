//
// Created by Dev on 8/29/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Graphics {
    struct CameraPacket;
}

namespace Djinn::Game {
    class ICameraRenderer : public COM::IQueryable {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the scene packet
        /// \return
        virtual Graphics::CameraPacket* GetPacket() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case kIID:
                    *out = this;
                    return COM::kOK;
            }

            return COM::kUnknownInterface;
        }
    };
}