//
// Created by Dev on 8/15/2018.
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