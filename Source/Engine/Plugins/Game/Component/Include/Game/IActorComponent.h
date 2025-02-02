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
 on 8/15/2018.
//
#pragma once

#include "IActorSystemInstance.h"

namespace Djinn::Game {
    class IActor;
    class IActorSystem;

    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT IActorComponent : public IActorSystemInstance {
    public:
        DJINN_COM_INTERFACE();

        /// Actor can write
        friend class IActor;

        IActorComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

    public:
        /// Destroy this component
        /// Destruction may be deferred to the end of the frame
        void Destroy();

        /// Get the owning actor
        /// \return
        IActor *GetActor() const {
            return owner;
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance*>(this);
                    return COM::kOK;
                case IActorComponent::kIID:
                    *out = static_cast<IActorComponent*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Invoked before actor component creation for initialization of static sub objects
        virtual void StaticInitialize() {};

        /// Invoked during actor component creation
        virtual void Created() {};

        /// Invoked during actor component destruction
        virtual void Destroyed() {};

        /// Invoked when this actor component has been enabled
        virtual void Enabled() {};

        /// Invoked when this actor component has been disabled
        virtual void Disabled() {};

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        /// Owning actor
        IActor *owner = nullptr;
    };
}