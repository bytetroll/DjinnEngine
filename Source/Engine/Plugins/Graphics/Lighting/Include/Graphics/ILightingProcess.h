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
 on 2/24/2018.
//
#pragma once

#include <Graphics/IProcess.h>
#include <Graphics/PassBuilder.h>

namespace Djinn::Graphics {
    struct CameraPacket;

    struct LightingResources {
        Texture direct;
    };

    class ILightingProcess : public IProcess {
    public:
        DJINN_COM_INTERFACE();

        ILightingProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IProcess(registry, outer, host) {

        }


        /// Build this process
        /// \param builder
        /// \param delta
        /// \return
        virtual COM::Result Build(CameraPacket* packet, const LightingResources& resources, Graphics::PassBuilder builder, double delta) = 0;

        /// Get resources associated with this process
        /// \return
        virtual const LightingResources& GetResources() = 0;

        /// Get packet
        /// \return
        virtual CameraPacket* GetPacket() = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case IProcess::kIID:
                    *out = static_cast<IProcess*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}