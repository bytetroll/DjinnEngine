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
 on 20/06/2018.
//

#pragma once

#include <Base/ISystem.h>
#include "Entity.h"

namespace Djinn::Game {
    class IEntityComponentHost;

    class IEntityComponent : public Base::ISystem {
    public:
        DJINN_COM_INTERFACE();

        IEntityComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISystem(registry, outer, host) {

        }

        /// Invoked during component creation
        /// \param entity
        /// \param component
        virtual void Created(const Entity& entity, const OpaqueEntityComponent &component) { /* Default */ }

        /// Invoked during component destruction
        /// \param entity
        /// \param component
        virtual void Destroyed(const Entity& entity, const OpaqueEntityComponent &component) { /* Default */ }

        /// Invoked during the enabling of a component
        /// \param entity
        /// \param component
        virtual void Enabled(const Entity& entity, const OpaqueEntityComponent &component) { /* Default */ }

        /// Invoking during the disabling of a component
        /// \param entity
        /// \param component
        virtual void Disabled(const Entity& entity, const OpaqueEntityComponent &component) { /* Default */ }

        /// Invoked during an update
        /// \param delta
        virtual void Update(double delta) { /*Default */}

        /// Invoked during end of an update cycle
        virtual void PostUpdate() { /*Default */}

        /// Get the component worldHost
        /// \return
        virtual IEntityComponentHost *GetHost() = 0;

        /// Override
        COM::Result Configure(Base::PipelineBuilder& builder) override {
            return COM::kOK;
        }

        /// Override
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case Base::ISystem::kIID:
                    *out = static_cast<Base::ISystem*>(this);
                    return COM::kOK;
                case IEntityComponent::kIID:
                    *out = static_cast<IEntityComponent*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };

    /// Helper
#define DJINN_GAME_COMPONENT(STATE) \
    DJINN_COM_CLASS(); \
    using ComponentState = STATE;
}