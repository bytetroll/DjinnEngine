//
// Created by Dev on 20/06/2018.
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